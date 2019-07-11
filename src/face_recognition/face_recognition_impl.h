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

#ifndef MACEKIT_SRC_FACE_RECOGNITION_IMPL_H_
#define MACEKIT_SRC_FACE_RECOGNITION_IMPL_H_

#include "macekit/face_recognition.h"
#include "mace/public/mace.h"

namespace mace_kit {

class FaceRecognitionImpl : public FaceRecognition {
 public:
  FaceRecognitionImpl(const FaceRecognitionContext &context);

  FaceRecognitionImpl(const FaceRecognitionImpl&) = delete;
  FaceRecognitionImpl& operator=(const FaceRecognitionImpl&) = delete;

  ~FaceRecognitionImpl() override = default;

  Status ComputeEmbedding(Mat &mat, std::vector<float> *embed) override;

  Status Compare(Mat &lhs, Mat &rhs, float *similarity) override;

 private:
  std::shared_ptr<mace::MaceEngine> mace_engine_;
  std::shared_ptr<float> mace_input_buffer_;
  std::map<std::string, mace::MaceTensor> mace_output_tensors_;
};

}  // namespace mace_kit

#endif  // MACEKIT_SRC_FACE_RECOGNITION_IMPL_H_
