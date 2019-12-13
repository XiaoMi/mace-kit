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

#include <string.h>
#include "gtest/gtest.h"
#include "macekit/speech_recognition.h"

namespace mace_kit {

class SpeechRecognitionTest : public ::testing::Test {
 public:
  SpeechRecognitionTest() {
  }

  ~SpeechRecognitionTest() {
  }
};

TEST_F(SpeechRecognitionTest, TestSpeechRecognitionDecode) {
::testing::internal::LogToStderr();
const char test_wav[] = "data/test/test-1.wav";
const char config[] = "data/asr-tiny/online.conf";
const char graph_fst[] = "data/asr-tiny/graph_tiny/HCLG.fst";
const char truth[] = "帮我 打电话给 幺 三 二 七 九 八 四 九 幺 幺 七 ";
const char *result = kaldi::MaceNnet3WavDecode(config,
                                               graph_fst,
                                               test_wav,
                                               "").data();
std::cout << "decode result: |" << result << std::endl;
std::cout << "truth: |" << truth << std::endl;
EXPECT_NE(strcmp(truth, result), 0);
}

}  // namespace mace_kit
