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

#ifndef INCLUDE_MACEKIT_PERSON_SEGMENTATION_H_
#define INCLUDE_MACEKIT_PERSON_SEGMENTATION_H_

#include "macekit/mat.h"
#include "macekit/type.h"
#include "macekit/export.h"
#include "macekit/status.h"
#include "macekit/context.h"

namespace mace_kit {

struct MACEKIT_EXPORT PersonSegmentationContext : Context {
};

struct MACEKIT_EXPORT PersonSegmentationResult {
  // front image with score.
  Mat score_map;
  // front image with label, 0 for background, 1 for person
  Mat label_map;
};

class MACEKIT_EXPORT PersonSegmentation {
 public:
  static Status Create(const PersonSegmentationContext &context,
                       PersonSegmentation **person_segmentation_ptr);

  PersonSegmentation() = default;
  PersonSegmentation(const PersonSegmentation&) = delete;
  PersonSegmentation& operator=(const PersonSegmentation&) = delete;

  virtual ~PersonSegmentation() = default;

  virtual Status Segment(const Mat &input,
                         PersonSegmentationResult *result) = 0;
};

}  // namespace mace_kit

#endif  // INCLUDE_MACEKIT_PERSON_SEGMENTATION_H_
