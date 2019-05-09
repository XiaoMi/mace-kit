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

#ifndef MACEKIT_SRC_UTIL_SSD_BBOX_H_
#define MACEKIT_SRC_UTIL_SSD_BBOX_H_

#include <cstdint>
#include <vector>

namespace mace_kit {
namespace util {

class SSDBbox {
 public:
  SSDBbox();
  ~SSDBbox();

  // Decode single feature layer localization regression output.
  // `feature_localization` is regressed output localization. Its size
  // should be `feature_size` x anchor_count x 4 (cy, cx, h, w).
  // `feature_size` equals to number of pixels for this feature layer.
  // `anchor_shapes` contains all the shape (height, width) of different
  // anchors according to anchor size and anchor ratio set during training.
  // `prior_scaling` is hyperparameter for encode width and height of bboxes.
  //
  // Encoding formula:
  // g^(cx) = [g(cx) - d(cx)] / d(w) / prior_scaling(cx)
  // g^(cy) = [g(cy) - d(cy)] / d(h) / prior_scaling(cy)
  // g^(w) = log[g(w) / d(w) / prior_scaling(w)]
  // g^(h) = log[g(h) / d(h) / prior_scaling(h)]
  //
  // where g^ is encoded localization, g is actual localization,
  // d is anchor localization.
  //
  // This implies decoding formula:
  // g(cx) = d(cx) + g^(cx) * d(w) * prior_scaling(cx)
  // g(cy) = d(cy) + g^(cy) * d(h) * prior_scaling(cy)
  // g(w) = exp[g^(w) * d(w) * prior_scaling(w)]
  // g(h) = exp[g^(h) * d(h) * prior_scaling(h)]
  //
  // So final decoded bbox location should be:
  // [g(cy) - g(h) / 2, g(cx) - g(w) / 2, g(cy) + g(h) / 2, g(cx) + g(w) / 2]
  //
  // Note: feature_localization should be both input and output,
  // and output should be clipped within [0, 1]
  //
  void Decode(float *feature_localization,
              int64_t feature_size,
              const std::vector<std::vector<float>> &anchor_shapes,
              const std::vector<float> &prior_scaling);

  // Get anchor bboxes height and with for a feature layer.
  void GetAnchorsShape(int64_t img_height, int64_t img_width,
                       int64_t min_size, int64_t max_size,
                       const std::vector<float> &anchor_ratios,
                       std::vector<std::vector<float>> *anchor_shapes);

  // First, select predicted bboxes with top-k scores,
  // second, do nms.
  void SelectTopAndNMS(const float *scores,
                       const float *localization,
                       float *output_scores,
                       float *output_localization,
                       int64_t top_k = -1,
                       float nms_threshold = 0.45);

 private:
  float CalJaccard(const std::vector<float> &lhs,
                   const std::vector<float> &rhs);
};

}  // namespace util
}  // namespace mace_kit

#endif  // MACEKIT_SRC_UTIL_SSD_BBOX_H_
