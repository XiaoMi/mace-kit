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

#include "macekit/mat.h"

#include <assert.h>
#include <malloc.h>
#include <numeric>
#include <memory>
#include <cstring>

namespace mace_kit {

template<class T>
struct DataTypeToEnum;

template<int VALUE>
struct EnumToDataType;

#define MACEKIT_MAPPING_DATA_TYPE_AND_ENUM(DATA_TYPE, ENUM_VALUE)  \
  template <>                                                      \
  struct DataTypeToEnum<DATA_TYPE> {                               \
    static int v() { return ENUM_VALUE; }                          \
    static constexpr int value = ENUM_VALUE;                       \
  };                                                               \
  template <>                                                      \
  struct EnumToDataType<ENUM_VALUE> {                              \
    typedef DATA_TYPE Type;                                        \
  };

MACEKIT_MAPPING_DATA_TYPE_AND_ENUM(float, DT_FLOAT32);
MACEKIT_MAPPING_DATA_TYPE_AND_ENUM(uint8_t, DT_UINT8);

#if defined(__ANDROID__)
// arm cache line
constexpr size_t kMemoryAlignment = 64;
#else
// 32 bytes = 256 bits (AVX512)
constexpr size_t kMemoryAlignment = 32;
#endif

Mat::Mat() : Mat({}, DT_FLOAT32, FM_RGB) {
}

Mat::Mat(const std::vector<int> &shape, int data_type, int data_format)
    : shape_(shape), data_type_(data_type) {
  // we only support rgb now
  assert(data_format == FM_RGB);
  Create(shape, data_type);
}

Mat::Mat(const std::vector<int> &shape,
         int data_type,
         int data_format,
         void *data) : shape_(shape), data_type_(data_type), data_(data) {
  assert(data_format == FM_RGB);
}

Mat::Mat(const Mat &mat)
    : shape_(mat.shape_),
      data_type_(mat.data_type_),
      data_(mat.data_),
      created_data_(mat.created_data_) {
}

const void *Mat::ptr() const {
  return data_;
}

void *Mat::ptr() {
  return data_;
}


void Mat::Create(const std::vector<int> &shape, int data_type) {
  shape_ = shape;
  data_type_ = data_type;
  int size =
      std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
  if (size > 0) {
    u_char  *data = nullptr;
    if (DataTypeToEnum<float>::value == data_type) {
      data = reinterpret_cast<u_char *>(memalign(kMemoryAlignment,
          size * sizeof(float)));
    } else if (DataTypeToEnum<uint8_t>::value == data_type) {
      data = reinterpret_cast<u_char *>(memalign(kMemoryAlignment, size));
    } else {
      assert(false);
    }

    assert(data != nullptr);
    created_data_.reset(data);
    data_ = created_data_.get();
  }
}

void Mat::CopyTo(Mat *mat) const {
  assert(mat->shape_ == shape_);
  assert(mat->data_ != nullptr);
  int size =
      std::accumulate(shape_.begin(), shape_.end(), 1, std::multiplies<int>());
  if (DataTypeToEnum<float>::value == data_type_) {
    memcpy(mat->data_, data_, sizeof(float) * size);
  } else if (DataTypeToEnum<uint8_t>::value == data_type_) {
    memcpy(mat->data_, data_, size);
  } else {
    assert(false);
  }
}

Mat Mat::Clone() const {
  Mat mat(shape_, data_type_, FM_RGB);
  CopyTo(&mat);
  return mat;
}

std::vector<int> Mat::shape() const {
  return shape_;
}

int Mat::data_type() const {
  return data_type_;
}

}  // namespace mace_kit
