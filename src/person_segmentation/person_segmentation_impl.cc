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


#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>

#include "src/person_segmentation/model/include/mace_engine_factory.h"
#include "src/person_segmentation/person_segmentation_impl.h"

namespace mace_kit {

namespace {

const std::vector<std::string> mace_input_nodes{
    "Input"
};

const std::vector<std::string> mace_output_nodes{
    "ResizeBilinear_1",
};

const std::vector<int64_t> mace_input_shape{
    {1, 513, 513, 3},
};

const std::vector<int64_t> mace_output_shape{
    {1, 513, 513, 2},
};

}  // namespace


PersonSegmentationImpl::PersonSegmentationImpl(
    const PersonSegmentationContext &context) {
  // Import MACE model.

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
        static_cast<mace::GPUPriorityHint>(
        mace::GPUPriorityHint::PRIORITY_LOW));
  }
#endif  // MACEKIT_ENABLE_OPENCL

  mace::CreateMaceEngineFromCode(
      "person_segmentation",
      nullptr,
      0,
      mace_input_nodes,
      mace_output_nodes,
      config,
      &mace_engine_);

  input_size_ = std::accumulate(mace_input_shape.begin(),
                                mace_input_shape.end(),
                                1,
                                std::multiplies<int64_t>());
  auto mace_input_buffer =
      std::shared_ptr<float>(new float[input_size_],
                             std::default_delete<float[]>());
  mace_input_tensors_[mace_input_nodes[0]] =
      mace::MaceTensor(mace_input_shape,
                       mace_input_buffer,
                       mace::DataFormat::NHWC);

  output_size_ = std::accumulate(mace_output_shape.begin(),
                                 mace_output_shape.end(),
                                 1,
                                 std::multiplies<int64_t>());
  auto mace_output_buffer =
      std::shared_ptr<float>(new float[output_size_],
                             std::default_delete<float[]>());
  mace_output_tensors_[mace_output_nodes[0]] =
      mace::MaceTensor(mace_output_shape,
                       mace_output_buffer,
                       mace::DataFormat::NHWC);
}

Status PersonSegmentationImpl::Segment(const Mat &input,
                                       PersonSegmentationResult *result) {
  int input_height = input.shape()[0];
  int input_width = input.shape()[1];
  int input_channel = input.shape()[2];

  if (input_height > mace_input_shape[1] || input_width > mace_input_shape[2]
      || input_channel != mace_input_shape[3]) {
    return Status::InvalidArgument("Input shape not match.");
  }

  // pre-process
  // pad and normalize
  auto input_buffer = mace_input_tensors_[mace_input_nodes[0]].data().get();
  auto input_data_ptr = input.data<float>();
  std::memset(input_buffer, 0, input_size_ * sizeof(float));
  float scale = 2.f / 255.f;
  int org_input_idx = 0;
  for (int h = 0; h < input_height; h++) {
    for (int w = 0; w < input_width; w++) {
      for (int c = 0; c < input_channel; c++) {
        int idx = (h * mace_input_shape[2] + w) * mace_input_shape[3] + c;
        input_buffer[idx] = scale * input_data_ptr[org_input_idx] - 1;
        org_input_idx++;
      }
    }
  }

  mace_engine_->Run(mace_input_tensors_, &mace_output_tensors_);

  // post-process
  auto output_buffer = mace_output_tensors_[mace_output_nodes[0]].data().get();

  auto score_map_ptr = result->score_map.data<float>();
  auto label_map_ptr = result->label_map.data<int>();

  int score_map_idx = 0;
  int label_map_idx = 0;
  for (int h = 0; h < input_height; h++) {
    for (int w = 0; w < input_width; w++) {
      int idx = (h * mace_output_shape[2] + w) * mace_output_shape[3];
      score_map_ptr[score_map_idx] = output_buffer[idx];
      score_map_ptr[score_map_idx + 1] = output_buffer[idx + 1];
      // argmax
      label_map_ptr[label_map_idx] = 0;
      if (output_buffer[idx+1] > output_buffer[idx]) {
        label_map_ptr[label_map_idx] = 1;
      }

      score_map_idx += 2;
      label_map_idx += 1;
    }
  }

  return Status::OK();
}

Status PersonSegmentation::Create(
    const PersonSegmentationContext &context,
    PersonSegmentation **person_segmentation_ptr) {
  *person_segmentation_ptr = new PersonSegmentationImpl(context);
  return Status::OK();
}

}  // namespace mace_kit
