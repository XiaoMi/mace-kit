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

#ifndef INCLUDE_MACEKIT_FACE_RECOGNITION_H_
#define INCLUDE_MACEKIT_FACE_RECOGNITION_H_

#include <vector>
#include <map>
#include "macekit/mat.h"
#include "macekit/type.h"
#include "macekit/export.h"
#include "macekit/status.h"
#include "macekit/context.h"

namespace mace_kit {

struct MACEKIT_EXPORT FaceRecognitionContext : Context {
};

class MACEKIT_EXPORT FaceRecognition {
 public:
  static Status Create(const FaceRecognitionContext &context,
                       FaceRecognition **face_recognition_ptr);

  FaceRecognition() = default;
  FaceRecognition(const FaceRecognition &) = delete;
  FaceRecognition &operator=(const FaceRecognition &) = delete;

  virtual ~FaceRecognition() = default;

  virtual Status ComputeEmbedding(Mat *mat, std::vector<float> *embed) = 0;

  virtual Status Compare(Mat *lhs, Mat *rhs, float *similarity) = 0;

};

}  // namespace mace_kit

#endif  // INCLUDE_MACEKIT_FACE_RECOGNITION_H_
