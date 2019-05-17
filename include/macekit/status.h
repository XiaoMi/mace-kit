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

#ifndef MACEKIT_INCLUDE_STATUS_H_
#define MACEKIT_INCLUDE_STATUS_H_

#include "macekit/export.h"
#include <string>

namespace mace_kit {

class MACEKIT_EXPORT Status {
 public:
  enum Code {
    kOk = 0,
    kInvalidArgument = 1,
    kRuntimeError = 2
  };

  Status() noexcept : code_(kOk), msg_("") {}
  ~Status() = default;

  Status(const Status &rhs) : code_(rhs.code_), msg_(rhs.msg_) {}
  Status &operator=(const Status &rhs) {
    if (this != &rhs) {
      code_ = rhs.code_;
      msg_ = rhs.msg_;
    }
    return *this;
  }

  static Status OK() { return Status(); }

  static Status InvalidArgument(const char *msg) {
    return Status(kInvalidArgument, msg);
  }

  static Status RuntimeError(const char *msg) {
    return Status(kRuntimeError, msg);
  }

  Code code() const {
    return code_;
  }

  bool ok() const {
    return code_ == kOk;
  }

  bool IsInvalidArgument() const {
    return code_ == kInvalidArgument;
  }

  bool IsRuntimeError() const {
    return code_ == kRuntimeError;
  }

 private:
  Status(Code code, const char *msg) : code_(code), msg_(msg) {}

  Code code_;
  std::string msg_;
};

}  // namespace mace_kit

#endif  // MACEKIT_INCLUDE_STATUS_H_
