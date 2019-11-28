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
#include "macekit/object_detection.h"

namespace mace_kit {

namespace {

std::vector<std::string> classes = {"background",
                                    "aeroplane", "bicycle", "bird", "boat",
                                    "bottle", "bus", "car", "cat", "chair",
                                    "cow", "diningtable", "dog", "horse",
                                    "motorbike", "person", "pottedplant",
                                    "sheep", "sofa", "train", "tvmonitor"};

void DrawAnchor(cv::InputOutputArray img,
                const ObjectResult &object_result) {
  auto &objects = object_result.objects;

  int height = img.rows(), width = img.cols();
  for (auto &object : objects) {
    cv::Rect rect(
        width * object.xmin,
        height * object.ymin,
        width * (object.xmax - object.xmin),
        height * (object.ymax - object.ymin));
    cv::rectangle(img, rect, cv::Scalar(250, 0, 0));
    cv::putText(img,
                classes[object.label] + ' ' + std::to_string(object.confidence),
                rect.tl(),
                cv::FONT_ITALIC,
                0.5,
                cv::Scalar(100, 0, 0));
  }
}

void ImgToMat(const cv::Mat &img, Mat *mat) {
  cv::Mat img_rgb;
  cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);

  cv::Mat img_fp32;
  img_rgb.convertTo(img_fp32, CV_32FC3);

  cv::Mat img_resized;
  const size_t img_size = 300;
  cv::resize(img_fp32, img_resized, cv::Size(img_size, img_size));

  mat->Create({img_size, img_size, 3}, DT_FLOAT32);
  memcpy(mat->ptr(), img_resized.data, img_size * img_size * 3 * sizeof(float));
}

}  // namespace
class ObjectDetectionTest : public ::testing::Test {
 public:
  ObjectDetectionTest() {
    ObjectDetectionContext context;
    context.device_type = DeviceType::GPU;
    context.cpu_affinity_policy = CPUAffinityPolicy::AFFINITY_NONE;
    context.thread_count = 4;

    ObjectDetection::Create(context, &object_detection_);
  }

  ~ObjectDetectionTest() {
    delete object_detection_;
  }

 protected:
  ObjectDetection *object_detection_;
};

TEST_F(ObjectDetectionTest, TestDetect) {
  ::testing::internal::LogToStderr();
  cv::Mat img = cv::imread("data/test/000067.jpg");
  Mat input;
  ImgToMat(img, &input);

  ObjectResult result;
  Status status = object_detection_->Detect(&input, 10, &result);
  EXPECT_TRUE(status.ok());

  DrawAnchor(img, result);
  cv::imwrite("test_out.png", img);
}

}  // namespace mace_kit
