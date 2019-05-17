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
#include "src/util/ssd_bbox.h"
#include "mace_engine_factory.h"

namespace mace_kit {

namespace {

const std::vector<std::string> mace_input_nodes{
    "input"
};

const std::vector<std::string> mace_output_nodes{
    "ssd_mobilenet_v2/layer_8/expansion_output_box/conv_cls/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/layer_15/expansion_output_box/conv_cls/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/layer_18/output_box/conv_cls/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/last_box/conv_cls/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/layer_8/expansion_output_box/conv_loc/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/layer_15/expansion_output_box/conv_loc/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/layer_18/output_box/conv_loc/BatchNorm/FusedBatchNorm",
    "ssd_mobilenet_v2/last_box/conv_loc/BatchNorm/FusedBatchNorm"
};

const std::vector<int> img_shape{
    300, 300
};

const std::vector<std::vector<int>> feature_shapes{
    {38, 38},
    {19, 19},
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

const float nms_threshold = 0.45f;

}  // namespace


FaceDetectionImpl::FaceDetectionImpl(const FaceDetectionContext &context) {
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
                    "/data/local/tmp/mace_run/interior" : storage_path_ptr);
    gpu_context = mace::GPUContextBuilder()
        .SetStoragePath(storage_path)
        .Finalize();
    config.SetGPUContext(gpu_context);
    config.SetGPUHints(
        static_cast<mace::GPUPerfHint>(mace::GPUPerfHint::PERF_NORMAL),
        static_cast<mace::GPUPriorityHint>(mace::GPUPriorityHint::PRIORITY_LOW));
  }
#endif  // MACEKIT_ENABLE_OPENCL

  mace::CreateMaceEngineFromCode("ssd_mobilenet_v2",
                                 nullptr,
                                 0,
                                 mace_input_nodes,
                                 mace_output_nodes,
                                 config,
                                 &mace_engine_);

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

    auto buffer_out = std::shared_ptr<float>(new float[cls_output_size],
                                             std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[i]] =
        mace::MaceTensor({cls_output_shape, buffer_out});
    buffer_out = std::shared_ptr<float>(new float[loc_output_size],
                                        std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[i + feature_shapes.size()]] =
        mace::MaceTensor({loc_output_shape, buffer_out});
  }
}

Status FaceDetectionImpl::Detect(Mat &mat,
                                 int max_face_count,
                                 FaceResult *result) {
  // Import MACE model and run.
  //
  // MACE model has n (feature_layers) x 2 (classification/bbox regression)
  // for MACE Kit model, we use n = 4.
  // output tensors:
  // classification outputs:
  // 1, hi, wi, anchor_count_per_pixel x 2 (background / face)
  // ...
  // bbox regression:
  // 1, hi, wi, anchor_count_per_pixel x 4 (cy, cx, h, w)

  std::vector<int64_t>
      input_shape{1, mat.shape()[0], mat.shape()[1], mat.shape()[2]};
  auto input_data = std::shared_ptr<float>(mat.data<float>(), [](float *) {});
  mace::MaceTensor mace_input_tensor(input_shape, input_data);
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
  util::SSDBbox ssd_bbox;

  for (int i = 0; i < feature_layer_count; i++) {
    std::vector<std::vector<float>> anchor_shapes;
    ssd_bbox.GetAnchorsShape(img_shape[0],
                             img_shape[2],
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
      scores[anchor_index + j] = scores_tensor[i][j * 2 + 1];
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
                           nms_threshold);

  result->faces.resize(output_scores.size());
  auto &faces = result->faces;
  for (size_t i = 0; i < output_scores.size(); i++) {
    faces[i].score = output_scores[i];
    faces[i].localization = output_localizations[i];
  }

  return Status::OK();
}

Status FaceDetection::Create(const FaceDetectionContext &context,
                             FaceDetection **face_detection_ptr) {
  *face_detection_ptr = new FaceDetectionImpl(context);
  return Status::OK();
}

}  // namespace mace_kit