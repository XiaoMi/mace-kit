// Copyright 2019 The MACE-KIT Authors. All Rights Reserved.
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

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>

#include "src/face_recognition/face_recognition_impl.h"
#include "src/face_recognition/model/include/mace_engine_factory.h"

namespace mace_kit {

namespace {

const std::vector<std::string> mace_input_nodes{
    "input"
};

const std::vector<std::string> mace_output_nodes{
    "face_recognition/MobileFaceNet/Logits/SpatialSqueeze"
};

const std::vector<int> img_shape{
    112, 112
};

const int embeding_size = 128;

}  // namespace


FaceRecognitionImpl::FaceRecognitionImpl(
    const FaceRecognitionContext &context) {
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
        static_cast<mace::GPUPriorityHint>(
            mace::GPUPriorityHint::PRIORITY_LOW));
  }
#endif  // MACEKIT_ENABLE_OPENCL

  mace::CreateMaceEngineFromCode("face_recognition",
                                 nullptr,
                                 0,
                                 mace_input_nodes,
                                 mace_output_nodes,
                                 config,
                                 &mace_engine_);

  mace_input_buffer_ =
      std::shared_ptr<float>(new float[img_shape[0] * img_shape[1] * 3],
                             std::default_delete<float[]>());
  auto buffer_out = std::shared_ptr<float>(new float[embeding_size],
                                           std::default_delete<float[]>());
  mace_output_tensors_[mace_output_nodes[0]] =
      mace::MaceTensor({embeding_size}, buffer_out);
}

Status FaceRecognitionImpl::ComputeEmbedding(Mat *mat,
                                             std::vector<float> *embed) {
  std::vector<int64_t>
      input_shape{1, mat->shape()[0], mat->shape()[1], mat->shape()[2]};

  // RGB -> substract mean
  int idx = 0;
  for (int h = 0; h < img_shape[0]; h++) {
    for (int w = 0; w < img_shape[1]; w++) {
      for (int c = 0; c < 3; c++) {
        mace_input_buffer_.get()[idx] =
            mat->data<float>()[idx] / 127.5f - 1.f;

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

  const float *data = mace_output_tensors_[mace_output_nodes[0]].data().get();
  embed->insert(embed->end(), data, data + embeding_size);

  return Status::OK();
}

Status FaceRecognitionImpl::Compare(Mat *lhs,
                                    Mat *rhs,
                                    float *similarity) {
  std::vector<float> lhs_embed, rhs_embed;
  ComputeEmbedding(lhs, &lhs_embed);
  ComputeEmbedding(rhs, &rhs_embed);
  float cross = 0.f, lhs_norm = 0.f, rhs_norm = 0.f;
  for (int i = 0; i < embeding_size; i++) {
    cross += lhs_embed[i] * rhs_embed[i];
    lhs_norm += lhs_embed[i] * lhs_embed[i];
    rhs_norm += rhs_embed[i] * rhs_embed[i];
  }
  const float epsilon = 1e-12;
  lhs_norm = std::sqrt(lhs_norm);
  rhs_norm = std::sqrt(rhs_norm);

  *similarity = cross / (lhs_norm * rhs_norm + epsilon);

  return Status::OK();
}

Status FaceRecognition::Create(const FaceRecognitionContext &context,
                               FaceRecognition **face_recognition_ptr) {
  *face_recognition_ptr = new FaceRecognitionImpl(context);
  return Status::OK();
}

}  // namespace mace_kit


