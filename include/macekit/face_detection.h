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

#ifndef INCLUDE_MACEKIT_FACE_DETECTION_H_
#define INCLUDE_MACEKIT_FACE_DETECTION_H_

#include <vector>
#include <map>
#include "macekit/mat.h"
#include "macekit/type.h"
#include "macekit/export.h"
#include "macekit/status.h"
#include "macekit/context.h"

namespace mace_kit {

struct MACEKIT_EXPORT FaceDetectionContext : Context {
};

struct MACEKIT_EXPORT Face {
  float score;
  std::vector<float> localization;
};

struct MACEKIT_EXPORT FaceResult {
  std::vector<Face> faces;
};

class MACEKIT_EXPORT FaceDetection {
 public:
  static Status Create(const FaceDetectionContext &context,
                       FaceDetection **face_detection_ptr);

  FaceDetection() = default;
  FaceDetection(const FaceDetection &) = delete;
  FaceDetection &operator=(const FaceDetection &) = delete;

  virtual ~FaceDetection() = default;

  virtual Status Detect(Mat *mat, int max_face_count, FaceResult *result) = 0;
};

}  // namespace mace_kit

#endif  // INCLUDE_MACEKIT_FACE_DETECTION_H_
