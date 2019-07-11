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
#include "opencv2/opencv.hpp"
#include "macekit/face_detection.h"

namespace mace_kit {

namespace {

void DrawAnchor(cv::InputOutputArray img,
                const FaceResult &face_result) {
  auto &faces = face_result.faces;

  int height = img.rows(), width = img.cols();
  for (auto &face : faces) {
    cv::Rect rect(
        width * face.localization[0],
        height * face.localization[1],
        width * (face.localization[2] - face.localization[0]),
        height * (face.localization[3] - face.localization[1]));
    cv::rectangle(img, rect, cv::Scalar(250, 0, 0));
    cv::putText(img,
                std::to_string(face.score),
                rect.tl(),
                cv::FONT_ITALIC,
                0.5,
                cv::Scalar(100, 0, 0));
  }
}

}  // namespace
class FaceDetectionTest : public ::testing::Test {
 public:
  FaceDetectionTest() {
    FaceDetectionContext context;
    context.device_type = DeviceType::CPU;
    context.cpu_affinity_policy = CPUAffinityPolicy::AFFINITY_NONE;
    context.thread_count = 4;

    FaceDetection::Create(context, &face_detection_);
  }

  ~FaceDetectionTest() {
    delete face_detection_;
  }

 protected:
  FaceDetection *face_detection_;
};

TEST_F(FaceDetectionTest, TestDetect) {
  ::testing::internal::LogToStderr();
  cv::Mat img = cv::imread("data/test/000003.jpg");

  cv::Mat img_rgb;
  cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);

  cv::Mat img_fp32;
  img_rgb.
  convertTo(img_fp32, CV_32FC3);

  cv::Mat img300_resized;
  cv::resize(img_fp32, img300_resized, cv::Size(300, 300));

  Mat input({300, 300, 3}, DT_FLOAT32, FM_RGB, img300_resized.data);
  FaceResult result;
  Status status = face_detection_->Detect(input, 500, &result);
  EXPECT_TRUE(status.ok());

  DrawAnchor(img, result);
  cv::imwrite("test_out.png", img);
}

}  // mace_kit