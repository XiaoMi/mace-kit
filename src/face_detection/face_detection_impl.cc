// Copyright 2019 The MACE Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/face_detection/face_detection_impl.h"

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>

#include "src/face_detection/model/include/mace_engine_factory.h"

namespace mace_kit {

namespace {

const std::vector<float> means{
    123.f,  // R
    117.f,  // G
    104.f   // B
};

const std::vector<std::string> mace_input_nodes{
    "input"
};

const std::vector<std::string> mace_output_nodes{
    "feat1_box/conv_cls/BatchNorm/FusedBatchNorm",
    "feat2_box/conv_cls/BatchNorm/FusedBatchNorm",
    "feat3_box/conv_cls/BatchNorm/FusedBatchNorm",
    "feat4_box/conv_cls/BatchNorm/FusedBatchNorm",
    "feat1_box/conv_loc/BatchNorm/FusedBatchNorm",
    "feat2_box/conv_loc/BatchNorm/FusedBatchNorm",
    "feat3_box/conv_loc/BatchNorm/FusedBatchNorm",
    "feat4_box/conv_loc/BatchNorm/FusedBatchNorm",
};

const std::vector<int> img_shape{
    320, 320
};

const std::vector<std::vector<int>> feature_shapes{
    {40, 40},
    {20, 20},
    {10, 10},
    {5, 5}
};

const std::vector<int> steps{
    8, 16, 32, 64
};

const std::vector<std::vector<int>> anchor_sizes{
    {16, 24},
    {32, 48},
    {64, 96},
    {128, 256}
};

const std::vector<std::vector<float>> anchor_ratios{
    {},
    {},
    {},
    {}
};

const std::vector<float> prior_scaling{
    0.1, 0.1, 0.2, 0.2
};

const float score_threshold = 0.5f;
const float nms_threshold = 0.45f;

}  // namespace


FaceDetectionImpl::FaceDetectionImpl(const FaceDetectionContext &context) {
  // Import MACE model.
  //
  // MACE model has n (feature_layers) x 2 (classification/bbox regression)
  // for MACE Kit model, we use n = 4.
  // output tensors:
  // classification outputs:
  // 1, hi, wi, anchor_count_per_pixel x 2 (background / face)
  // ...
  // bbox regression:
  // 1, hi, wi, anchor_count_per_pixel x 4 (cy, cx, h, w)

  mace::MaceStatus status;
  mace::MaceEngineConfig
      config(static_cast<mace::DeviceType>(context.device_type));
  status = config.SetCPUThreadPolicy(
      context.thread_count,
      static_cast<mace::CPUAffinityPolicy>(context.cpu_affinity_policy));
  if (status != mace::MaceStatus::MACE_SUCCESS) {
    std::cerr << "Set openmp or cpu affinity failed." << std::endl;
  }

#ifdef MACEKIT_ENABLE_OPENCL
  std::shared_ptr<mace::GPUContext> gpu_context;
  if (context.device_type == DeviceType::GPU) {
    // DO NOT USE tmp directory.
    // Please use APP's own directory and make sure the directory exists.
    const char *storage_path_ptr = getenv("MACE_INTERNAL_STORAGE_PATH");
    const std::string storage_path =
        std::string(storage_path_ptr == nullptr ?
                    "/data/local/tmp" : storage_path_ptr);
    gpu_context = mace::GPUContextBuilder()
        .SetStoragePath(storage_path)
        .Finalize();
    config.SetGPUContext(gpu_context);
    config.SetGPUHints(
        static_cast<mace::GPUPerfHint>(mace::GPUPerfHint::PERF_NORMAL),
        static_cast<mace::GPUPriorityHint>(mace::GPUPriorityHint::PRIORITY_LOW));
  }
#endif  // MACEKIT_ENABLE_OPENCL

  mace::CreateMaceEngineFromCode("face_detection",
                                 nullptr,
                                 0,
                                 mace_input_nodes,
                                 mace_output_nodes,
                                 config,
                                 &mace_engine_);

  mace_input_buffer_ =
      std::shared_ptr<float>(new float[img_shape[0] * img_shape[1] * 3],
                             std::default_delete<float[]>());

  for (size_t i = 0; i < feature_shapes.size(); i++) {
    int64_t anchor_count = anchor_sizes[i].size() + anchor_ratios[i].size();
    std::vector<int64_t> cls_output_shape{
        1, feature_shapes[i][0], feature_shapes[i][1], anchor_count * 2
    };
    int64_t cls_output_size =
        std::accumulate(cls_output_shape.begin(), cls_output_shape.end(), 1,
                        std::multiplies<int64_t>());
    std::vector<int64_t> loc_output_shape{
        1, feature_shapes[i][0], feature_shapes[i][1], anchor_count * 4
    };
    int64_t loc_output_size =
        std::accumulate(loc_output_shape.begin(), loc_output_shape.end(), 1,
                        std::multiplies<int64_t>());

    auto cls_buffer_out = std::shared_ptr<float>(new float[cls_output_size],
                                                 std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[i]] =
        mace::MaceTensor({cls_output_shape, cls_buffer_out});
    auto loc_buffer_out = std::shared_ptr<float>(new float[loc_output_size],
                                                 std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[i + feature_shapes.size()]] =
        mace::MaceTensor({loc_output_shape, loc_buffer_out});
  }
}

Status FaceDetectionImpl::Detect(Mat &mat,
                                 int max_face_count,
                                 FaceResult *result) {
  std::vector<int64_t>
      input_shape{1, mat.shape()[0], mat.shape()[1], mat.shape()[2]};

  // RGB -> substract mean
  int idx = 0;
  for (int h = 0; h < img_shape[0]; h++) {
    for (int w = 0; w < img_shape[1]; w++) {
      for (int c = 0; c < 3; c++) {
        mace_input_buffer_.get()[idx] = mat.data<float>()[idx] - means[c];
        idx++;
      }
    }
  }

  mace::MaceTensor
      mace_input_tensor
      (input_shape, mace_input_buffer_, mace::DataFormat::NHWC);
  std::map<std::string, mace::MaceTensor> mace_input_tensors{
      {mace_input_nodes[0], mace_input_tensor}
  };

  mace_engine_->Run(mace_input_tensors, &mace_output_tensors_);

  int feature_layer_count = feature_shapes.size();
  std::vector<const float *> localizations_tensor(feature_layer_count);
  std::vector<const float *> scores_tensor(feature_layer_count);

  for (int i = 0; i < feature_layer_count; i++) {
    scores_tensor[i] = mace_output_tensors_[mace_output_nodes[i]].data().get();
    localizations_tensor[i] = mace_output_tensors_[mace_output_nodes[i
        + feature_layer_count]].data().get();
  }

  int anchor_count = 0;
  for (int i = 0; i < feature_layer_count; i++) {
    anchor_count += feature_shapes[i][0] * feature_shapes[i][1]
        * (anchor_sizes[i].size() + anchor_ratios[i].size());
  }

  std::vector<float> scores(anchor_count);
  std::vector<float> localizations(anchor_count * 4);
  int anchor_index = 0;

  for (int i = 0; i < feature_layer_count; i++) {
    std::vector<std::vector<float>> anchor_shapes;
    ssd_bbox.GetAnchorsShape(img_shape[0],
                             img_shape[1],
                             anchor_sizes[i][0],
                             anchor_sizes[i][1],
                             anchor_ratios[i],
                             &anchor_shapes);

    ssd_bbox.Decode(localizations_tensor[i],
                    feature_shapes[i],
                    img_shape,
                    steps[i],
                    anchor_shapes,
                    prior_scaling,
                    &localizations[anchor_index * 4],
                    0.5);

    int anchor_count =
        feature_shapes[i][0] * feature_shapes[i][1] * anchor_shapes.size();

    for (int j = 0; j < anchor_count; j++) {
      // FIXME: MACE does not support multi-dim softmax
      float face_cls = std::exp(scores_tensor[i][j * 2 + 1]);
      scores[anchor_index + j] =
          face_cls / (face_cls + std::exp(scores_tensor[i][j * 2]));
    }

    anchor_index += anchor_count;
  }

  std::vector<float> output_scores;
  std::vector<std::vector<float>> output_localizations;

  ssd_bbox.SelectTopAndNMS(scores.data(),
                           localizations.data(),
                           anchor_count,
                           &output_scores,
                           &output_localizations,
                           max_face_count,
                           score_threshold,
                           nms_threshold);

  result->faces.resize(output_scores.size());
  auto &faces = result->faces;
  for (size_t i = 0; i < output_scores.size(); i++) {
    faces[i].score = output_scores[i];
    faces[i].localization = {output_localizations[i][0],
                             output_localizations[i][1],
                             output_localizations[i][2],
                             output_localizations[i][3]};
  }

  return Status::OK();
}

Status FaceDetection::Create(const FaceDetectionContext &context,
                             FaceDetection **face_detection_ptr) {
  *face_detection_ptr = new FaceDetectionImpl(context);
  return Status::OK();
}

}  // namespace mace_kit