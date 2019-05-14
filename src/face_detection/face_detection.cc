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

#include "macekit/face_detection.h"
#include "macekit/ssd_bbox.h"

namespace mace_kit {
namespace util {

FaceDetection::FaceDetection(const FaceDetectionContext &context) {

}

void FaceDetection::Detect(Mat &mat,
                           int max_face_count,
                           FaceResult *result) {
  // Import MACE model and run.
  //
  // MACE model has n (feature_layers) x 2 (classification/bbox regression)
  // for MACE Kit model, we use n = 4.
  // output tensors:
  // classification outputs:
  // 1, hi, wi, anchor_count_per_pixel x 2 (background / face)
  // ...
  // bbox regression:
  // 1, hi, wi, anchor_count_per_pixel x 4 (cy, cx, h, w)
  const float *localizations_tensor[4];
  const float *scores_tensor[4];

  std::vector<int> img_shape{
      300, 300
  };

  std::vector<std::vector<int>> feature_shapes{
      {38, 38},
      {19, 19},
      {10, 10},
      {5, 5}
  };

  std::vector<int> steps{
      8, 16, 32, 64
  };

  std::vector<std::vector<int>> anchor_sizes{
      {16, 24},
      {32, 48},
      {64, 96},
      {128, 256}
  };

  std::vector<std::vector<float>> anchor_ratios{
      {},
      {},
      {},
      {}
  };

  std::vector<float> prior_scaling{
      0.1, 0.1, 0.2, 0.2
  };

  float nms_threshold = 0.45f;

  int feature_layer_count = feature_shapes.size();
  int anchor_count = 0;
  for (int i = 0; i < feature_layer_count; i++) {
    anchor_count += feature_shapes[i][0] * feature_shapes[i][1]
        * (anchor_sizes.size() + anchor_ratios.size());
  }

  std::vector<float> scores(anchor_count);
  std::vector<float> localizations(anchor_count * 4);
  int anchor_index = 0;
  SSDBbox ssd_bbox;

  for (int i = 0; i < feature_layer_count; i++) {
    std::vector<std::vector<float>> anchor_shapes;
    ssd_bbox.GetAnchorsShape(img_shape[0],
                             img_shape[2],
                             anchor_sizes[i][0],
                             anchor_sizes[i][1],
                             anchor_ratios[i],
                             &anchor_shapes);
    ssd_bbox.Decode(localizations_tensor[i],
                    feature_shapes[i],
                    img_shape,
                    steps[i],
                    anchor_shapes,
                    prior_scaling,
                    &localizations[anchor_index * 4],
                    0.5);
    int anchor_count =
        feature_shapes[i][0] * feature_shapes[i][1] * anchor_shapes.size();

    for (int j = 0; j < anchor_count; j++) {
      scores[anchor_index + j] = scores_tensor[i][j * 2 + 1];
    }

    anchor_index += anchor_count;
  }

  std::vector<float> output_scores;
  std::vector<std::vector<float>> output_localizations;

  ssd_bbox.SelectTopAndNMS(scores.data(),
                           localizations.data(),
                           anchor_count,
                           &output_scores,
                           &output_localizations,
                           max_face_count,
                           nms_threshold);

  result->faces.resize(output_scores.size());
  auto &faces = result->faces;
  for (int i = 0; i < output_scores.size(); i++) {
    faces[i].score = output_scores[i];
    faces[i].localization = output_localizations[i];
  }
}

}  // namespace util
}  // namespace mace_kit