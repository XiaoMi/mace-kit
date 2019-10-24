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

// This file defines core MACE APIs.
// There APIs will be stable and backward compatible.

#ifndef MACE_KALDI_MACE_H_
#define MACE_KALDI_MACE_H_

#include <string>

#ifndef KALDI_MACE_API
#define KALDI_MACE_API __attribute__((visibility("default")))
#endif
namespace kaldi {

KALDI_MACE_API std::string MaceNnet3WavDecode(
    const std::string &configure_file,
    const std::string &fst_filename,
    const std::string &wav_file,
    const std::string &prediction_file,
    const std::string &log_level="");
}

#endif  // MACE_KALDI_MACE_H_

