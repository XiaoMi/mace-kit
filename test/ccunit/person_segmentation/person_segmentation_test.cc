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
#include "macekit/person_segmentation.h"

namespace mace_kit {

class PersonSegmentationTest : public ::testing::Test {
 public:
  PersonSegmentationTest() {
    PersonSegmentationContext context;
    context.device_type = DeviceType::CPU;
    context.cpu_affinity_policy = CPUAffinityPolicy::AFFINITY_NONE;
    context.thread_count = 4;

    PersonSegmentation::Create(context, &person_segmentation_);
  }

  ~PersonSegmentationTest() {
    delete person_segmentation_;
  }

 protected:
  PersonSegmentation *person_segmentation_;
};

TEST_F(PersonSegmentationTest, TestSegment) {
  ::testing::internal::LogToStderr();
  int model_input_size = 513;
  cv::Mat img = cv::imread("data/test/000001.jpg");
  int img_height = img.size().height;
  int img_width = img.size().width;

  cv::Mat img_rgb;
  cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);

  cv::Mat img_fp32;
  img_rgb.convertTo(img_fp32, CV_32FC3);

  int resized_height = img_height, resized_width = img_width;
  if (img_height > model_input_size || img_width > model_input_size) {
    float scale_ratio = model_input_size * 1.0f /
        std::max(img_height, img_width);
    resized_height = static_cast<int>(img_height * scale_ratio);
    resized_width = static_cast<int>(img_width * scale_ratio);
  }
  cv::Mat resized_img;
  cv::resize(img_fp32, resized_img, cv::Size(resized_width, resized_height));

  Mat input({resized_height, resized_width, 3},
            DT_FLOAT32,
            FM_RGB,
            resized_img.data);
  PersonSegmentationResult result;
  int score_map_data_size = resized_height * resized_width * 2;
  auto score_map_data =
      std::shared_ptr<float>(new float[score_map_data_size],
                             std::default_delete<float[]>());
  result.score_map = Mat({resized_height, resized_width, 2},
                         DT_FLOAT32,
                         FM_RGB,
                         score_map_data.get());
  int label_map_data_size = resized_height * resized_width;
  auto label_map_data =
      std::shared_ptr<int>(new int[label_map_data_size],
                           std::default_delete<int[]>());
  result.label_map = Mat({resized_height, resized_width, 1},
                         DT_INT32,
                         FM_RGB,
                         label_map_data.get());

  Status status = person_segmentation_->Segment(input, &result);
  EXPECT_TRUE(status.ok());

  std::vector<unsigned char> mask_data(resized_height * resized_width * 3, 0);
  for (int i = 0; i < label_map_data_size; ++i) {
    if (label_map_data.get()[i] == 1) {
      mask_data[i * 3] = 128;
    }
  }

  cv::Mat mask = cv::Mat(resized_height,
                         resized_width,
                         CV_8UC3,
                         mask_data.data());
  cv::imwrite("segmentation.png", mask);
}

}  // namespace mace_kit
