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

#ifndef SRC_PERSON_SEGMENTATION_PERSON_SEGMENTATION_IMPL_H_
#define SRC_PERSON_SEGMENTATION_PERSON_SEGMENTATION_IMPL_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "macekit/person_segmentation.h"
#include "mace/public/mace.h"

namespace mace_kit {

class PersonSegmentationImpl : public PersonSegmentation {
 public:
  explicit PersonSegmentationImpl(const PersonSegmentationContext &context);

  PersonSegmentationImpl(const PersonSegmentationImpl&) = delete;
  PersonSegmentationImpl& operator=(const PersonSegmentationImpl&) = delete;

  ~PersonSegmentationImpl() override = default;

  Status Segment(const Mat &input, PersonSegmentationResult *result) override;

 private:
  std::shared_ptr<mace::MaceEngine> mace_engine_;
  int64_t input_size_;
  int64_t output_size_;
  std::map<std::string, mace::MaceTensor> mace_input_tensors_;
  std::map<std::string, mace::MaceTensor> mace_output_tensors_;
};

}  // namespace mace_kit

#endif  // SRC_PERSON_SEGMENTATION_PERSON_SEGMENTATION_IMPL_H_
