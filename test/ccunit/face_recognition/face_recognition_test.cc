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

#include <iostream>

#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include "macekit/face_recognition.h"

namespace mace_kit {

class FaceRecognitionTest : public ::testing::Test {
 public:
  FaceRecognitionTest() {
    FaceRecognitionContext context;
    context.device_type = DeviceType::CPU;
    context.cpu_affinity_policy = CPUAffinityPolicy::AFFINITY_NONE;
    context.thread_count = 4;

    FaceRecognition::Create(context, &face_recognition_);
  }

  ~FaceRecognitionTest() {
    delete face_recognition_;
  }

 protected:
  FaceRecognition *face_recognition_;
};

namespace {
  void ImgToMat(const std::string &img_path, Mat *mat) {
    cv::Mat img = cv::imread(img_path);

    cv::Mat img_rgb;
    cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);

    cv::Mat img_fp32;
    img_rgb.convertTo(img_fp32, CV_32FC3);

    cv::Mat img112_resized;
    cv::resize(img_fp32, img112_resized, cv::Size(112, 112));

    mat->Create({112, 112, 3}, DT_FLOAT32);
    memcpy(mat->ptr(), img112_resized.data, 112 * 112 * 3 * sizeof(float));
  }

  void PrintEmbedding(const std::vector<float> &embed) {
    std::cout << "Embedding: " << std::endl;
    for (float e : embed) {
      std::cout << e << " ";
    }
    std::cout << std::endl;
  }
}

TEST_F(FaceRecognitionTest, TestComputeEmbedding) {
  ::testing::internal::LogToStderr();
  Mat input1;
  ImgToMat("data/test/Anthony_Hopkins_0001.jpg", &input1);
  std::vector<float> embed1;

  Status status1 = face_recognition_->ComputeEmbedding(input1, &embed1);
  EXPECT_TRUE(status1.ok());
  PrintEmbedding(embed1);

  Mat input2;
  ImgToMat("data/test/Anthony_Hopkins_0002.jpg", &input2);
  std::vector<float> embed2;
  Status status2 = face_recognition_->ComputeEmbedding(input2, &embed2);
  EXPECT_TRUE(status2.ok());
  PrintEmbedding(embed2);
}

TEST_F(FaceRecognitionTest, TestCompare) {
  ::testing::internal::LogToStderr();
  Mat input1, input2;
  ImgToMat("data/test/Anthony_Hopkins_0001.jpg", &input1);
  ImgToMat("data/test/Anthony_Hopkins_0002.jpg", &input2);
  float similarity = 0.f;
  Status status = face_recognition_->Compare(input1, input2, &similarity);
  EXPECT_TRUE(status.ok() && similarity > 0.5);
  std::cout << "Similarity : " << similarity << std::endl;
}

}  // mace_kit