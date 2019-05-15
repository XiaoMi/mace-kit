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

#include "opencv2/opencv.hpp"

namespace mace_kit {
namespace face_detection {

void DrawAnchor(cv::InputOutputArray img,
                const std::vector<float> &scores,
                const std::vector<float> &localization) {
  for (size_t i = 0; i < scores.size(); i++) {
    cv::Rect rect
        (localization[i * 4 + 0],
         localization[i * 4 + 1],
         localization[i * 4 + 2],
         localization[i * 4 + 3]);
    cv::rectangle(img, rect, cv::Scalar(250, 0, 0));
    cv::putText(img,
                std::to_string(scores[i]),
                rect.tl(),
                cv::FONT_ITALIC,
                0.5,
                cv::Scalar(100, 0, 0));
  }
}

}  // namespace face_detection
}  // namespace mace_kit

int main() {
  cv::Mat img = cv::imread("../data/test.jpg");
  mace_kit::face_detection::DrawAnchor(img, {0.5, 0.7}, {50, 50, 100, 100});
  cv::imwrite("out.png", img);
  return 0;
}