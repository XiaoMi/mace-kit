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

#include "gtest/gtest.h"
#include "src/util/ssd_bbox.h"

namespace mace_kit {
namespace util {

class SSDBboxTest : public ::testing::Test {
 protected:
  SSDBbox ssd_bbox_;
};

TEST_F(SSDBboxTest, TestJaccard) {
  float l[4] = {0, 0, 2, 2};
  float r[4] = {1, 1, 3, 3};
  EXPECT_NEAR(1.f / 7, ssd_bbox_.CalJaccard(l, r), 1e-6);
}

TEST_F(SSDBboxTest, TestGetAnchorShape) {
  std::vector<std::vector<float>> anchor_shapes;
  ssd_bbox_.GetAnchorsShape(300, 300, 64, 128, {2, 0.5}, &anchor_shapes);
  std::vector<std::vector<float>> expected_anchor_shapes{
      {0.21333333333333335f, 0.21333333333333335f},
      {0.3016988933062603f, 0.3016988933062603f},
      {0.15084944665313013f, 0.3016988933062603f},
      {0.3016988933062603f, 0.15084944665313013f}
  };
  EXPECT_EQ(expected_anchor_shapes.size(), anchor_shapes.size());
  for (size_t i = 0; i < expected_anchor_shapes.size(); i++) {
    EXPECT_NEAR(expected_anchor_shapes[i][0], anchor_shapes[i][0], 1e-3);
    EXPECT_NEAR(expected_anchor_shapes[i][1], anchor_shapes[i][1], 1e-3);
  }

}

TEST_F(SSDBboxTest, TestNMS) {
  ::testing::internal::LogToStderr();
  float scores[3] = {1, 2, 3};
  float localization[12] = {
      0, 0, 2, 2,
      1, 1, 3, 3,
      0.2, 0.2, 2.2, 2.2
  };
  std::vector<float> output_scores;
  std::vector<std::vector<float>> output_localization;
  ssd_bbox_.SelectTopAndNMS(scores,
                            localization,
                            3,
                            &output_scores,
                            &output_localization,
                            2,
                            0.45);
  std::vector<float> expected_output_scores{2, 3};
  std::vector<std::vector<float>> expected_output_localization{
      {1, 1, 3, 3},
      {0.2, 0.2, 2.2, 2.2}
  };

  EXPECT_EQ(2, output_scores.size());
  for (size_t i = 0; i == expected_output_scores.size(); i++) {
    EXPECT_NEAR(expected_output_scores[i], output_scores[i], 1e-3);
  }
  for (size_t i = 0; i == expected_output_localization.size(); i++) {
    for (size_t j = 0; j < 4; j++) {
      EXPECT_NEAR(expected_output_localization[i][j],
                  output_localization[i][j],
                  1e-3);
    }
  }
}

}  // namespace util
}  // mace_kit