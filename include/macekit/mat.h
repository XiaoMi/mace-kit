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

#ifndef INCLUDE_MACEKIT_MAT_H_
#define INCLUDE_MACEKIT_MAT_H_

#include <vector>
#include <memory>
#include "macekit/export.h"

namespace mace_kit {

enum {
  FM_RGB
};

enum {
  DT_FLOAT32,
  DT_UINT8
};

class MACEKIT_EXPORT Mat {
 public:
  Mat();

  Mat(const std::vector<int> &shape, int data_type, int data_format);

  Mat(const std::vector<int> &shape,
      int data_type,
      int data_format,
      void *data);

  Mat(const Mat &mat);

  const void *ptr() const;

  void *ptr();

  template<typename T>
  const T *data() const {
    return reinterpret_cast<const T *>(ptr());
  }

  template<typename T>
  T *data() {
    return reinterpret_cast<T *>(ptr());
  }

  void Create(const std::vector<int> &shape, int data_type);

  void CopyTo(Mat *mat) const;

  Mat Clone() const;

  std::vector<int> shape() const;

  int data_type() const;

 private:
  std::vector<int> shape_;
  int data_type_;
  void *data_;
  std::shared_ptr<void> created_data_;
};

}  // namespace mace_kit

#endif  // INCLUDE_MACEKIT_MAT_H_
