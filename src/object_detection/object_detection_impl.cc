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

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <map>
#include <memory>
#include <algorithm>

#include "src/object_detection/model/include/mace_engine_factory.h"
#include "src/object_detection/object_detection_impl.h"

namespace mace_kit {

namespace {

const std::vector<float> means{
    127.5,  // R
    127.5,  // G
    127.5  // B
};

const std::vector<std::string> mace_input_nodes{
    "data"
};

const std::vector<std::string> mace_output_nodes{
    "mbox_loc",
    "mbox_conf_flatten",
    "mbox_priorbox"
};

const std::vector<int> img_shape{
    300, 300
};

const float scale = 0.007843;
const float confidence_threshold = 0.25f;
const float nms_threshold = 0.45f;
const int num_classes = 21;
const int top_k = 100;
const int num_prior = 1917;

inline float overlap(const Object &a, const Object &b) {
    if (a.xmin > b.xmax || a.xmax < b.xmin ||
        a.ymin > b.ymax || a.ymax < b.ymin) {
        return 0.f;
    }
    float overlap_w = std::min(a.xmax, b.xmax) - std::max(a.xmin, b.xmin);
    float overlap_h = std::min(a.ymax, b.ymax) - std::max(a.ymin, b.ymin);
    return overlap_w * overlap_h;
}
void NmsSortedBboxes(const std::vector<Object> &bboxes,
                     const float nms_threshold,
                     const int top_k,
                     std::vector<Object> *sorted_boxes) {
    const int n = std::min(top_k, static_cast<int>(bboxes.size()));
    std::vector<int> picked;

    std::vector<float> areas(n);
    for (int i = 0; i < n; ++i) {
        const Object &r = bboxes[i];
        float width = std::max(0.f, r.xmax - r.xmin);
        float height = std::max(0.f, r.ymax - r.ymin);
        areas[i] = width * height;
    }

    for (int i = 0; i < n; ++i) {
        const Object &a = bboxes[i];
        int keep = 1;
        for (size_t j = 0; j < picked.size(); ++j) {
            const Object &b = bboxes[picked[j]];

            float inter_area = overlap(a, b);
            float union_area = areas[i] + areas[picked[j]] - inter_area;
            if (inter_area / union_area > nms_threshold) {
                keep = 0;
                break;
            }
        }

        if (keep) {
            picked.push_back(i);
            sorted_boxes->push_back(bboxes[i]);
        }
    }
}

inline bool cmp(const Object &a, const Object &b) {
    return a.confidence > b.confidence;
}
}  // namespace


ObjectDetectionImpl::ObjectDetectionImpl(
    const ObjectDetectionContext &context) {
    // Import MACE model.
    //
    // output tensors:
    // classification outputs:
    // 1, hi, wi, anchor_count_per_pixel x 21 (background / object)
    // ...
    // bbox regression:
    // 1, hi, wi, anchor_count_per_pixel x 4 (cy, cx, h, w)

    mace::MaceStatus status;
    mace::MaceEngineConfig
        config(static_cast<mace::DeviceType>(context.device_type));
    status = config.SetCPUThreadPolicy(
        context.thread_count,
        static_cast<mace::CPUAffinityPolicy>(context.cpu_affinity_policy));
    if (status != mace::MaceStatus::MACE_SUCCESS) {
        std::cerr << "Set openmp or cpu affinity failed." << std::endl;
    }

#ifdef MACEKIT_ENABLE_OPENCL
    std::shared_ptr<mace::GPUContext> gpu_context;
  if (context.device_type == DeviceType::GPU) {
    // DO NOT USE tmp directory.
    // Please use APP's own directory and make sure the directory exists.
    const char *storage_path_ptr = getenv("MACE_INTERNAL_STORAGE_PATH");
    const std::string storage_path =
        std::string(storage_path_ptr == nullptr ?
                    "/data/local/tmp" : storage_path_ptr);
    gpu_context = mace::GPUContextBuilder()
        .SetStoragePath(storage_path)
        .Finalize();
    config.SetGPUContext(gpu_context);
    config.SetGPUHints(
        static_cast<mace::GPUPerfHint>(mace::GPUPerfHint::PERF_NORMAL),
        static_cast<mace::GPUPriorityHint>(
            mace::GPUPriorityHint::PRIORITY_LOW));
  }
#endif  // MACEKIT_ENABLE_OPENCL

    mace::CreateMaceEngineFromCode("object_detection",
                                   nullptr,
                                   0,
                                   mace_input_nodes,
                                   mace_output_nodes,
                                   config,
                                   &mace_engine_);

    mace_input_buffer_ =
        std::shared_ptr<float>(new float[img_shape[0] * img_shape[1] * 3],
                               std::default_delete<float[]>());
    std::vector<int64_t> loc_output_shape{1, num_prior * 4};
    auto loc_buffer_out =
        std::shared_ptr<float>(new float[num_prior * 4],
                               std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[0]] =
        mace::MaceTensor({loc_output_shape, loc_buffer_out});

    std::vector<int64_t> conf_output_shape{1, num_prior * num_classes};
    auto conf_buffer_out =
        std::shared_ptr<float>(new float[num_prior * num_classes],
                               std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[1]] =
        mace::MaceTensor({conf_output_shape, conf_buffer_out});

    std::vector<int64_t> pbox_output_shape{1, 2, num_prior * 4};
    auto pbox_buffer_out =
        std::shared_ptr<float>(new float[2* num_prior * 4],
                               std::default_delete<float[]>());
    mace_output_tensors_[mace_output_nodes[2]] =
        mace::MaceTensor({pbox_output_shape, pbox_buffer_out});
}

Status ObjectDetectionImpl::Detect(Mat *mat,
                                   int keep_top_k,
                                   ObjectResult *result) {
    std::vector<int64_t>
        input_shape{1, mat->shape()[0], mat->shape()[1], mat->shape()[2]};

    // RGB -> substract mean
    int idx = 0;
    for (int h = 0; h < img_shape[0]; h++) {
        for (int w = 0; w < img_shape[1]; w++) {
            for (int c = 0; c < 3; c++) {
                mace_input_buffer_.get()[idx] =
                    (mat->data<float>()[idx] - means[c]) * scale;
                idx++;
            }
        }
    }

    mace::MaceTensor
        mace_input_tensor
        (input_shape, mace_input_buffer_, mace::DataFormat::NHWC);
    std::map<std::string, mace::MaceTensor> mace_input_tensors{
        {mace_input_nodes[0], mace_input_tensor}
    };

    mace_engine_->Run(mace_input_tensors, &mace_output_tensors_);

    const float *loc_ptr =
        mace_output_tensors_[mace_output_nodes[0]].data().get();
    const float *conf_ptr =
        mace_output_tensors_[mace_output_nodes[1]].data().get();
    const float *pbox_ptr =
        mace_output_tensors_[mace_output_nodes[2]].data().get();

    std::vector<float> bboxes(4 * num_prior);
    for (int i = 0; i < num_prior; ++i) {
        int index = i * 4;
        const float *lc = loc_ptr + index;
        const float *pb = pbox_ptr + index;
        const float *var = pb + num_prior * 4;

        float pb_w = pb[2] - pb[0];
        float pb_h = pb[3] - pb[1];
        float pb_cx = (pb[0] + pb[2]) * 0.5f;
        float pb_cy = (pb[1] + pb[3]) * 0.5f;

        float bbox_cx = var[0] * lc[0] * pb_w + pb_cx;
        float bbox_cy = var[1] * lc[1] * pb_h + pb_cy;
        float bbox_w = std::exp(var[2] * lc[2]) * pb_w;
        float bbox_h = std::exp(var[3] * lc[3]) * pb_h;

        bboxes[0 + index] = bbox_cx - bbox_w * 0.5f;
        bboxes[1 + index] = bbox_cy - bbox_h * 0.5f;
        bboxes[2 + index] = bbox_cx + bbox_w * 0.5f;
        bboxes[3 + index] = bbox_cy + bbox_h * 0.5f;
    }
    // start from 1 to ignore background class

    for (int i = 1; i < num_classes; ++i) {
        // filter by confidence threshold
        std::vector<Object> class_bbox_rects;
        for (int j = 0; j < num_prior; ++j) {
            float confidence = conf_ptr[j * num_classes + i];
            if (confidence > confidence_threshold) {
                Object c = {bboxes[0 + j * 4],
                            bboxes[1 + j * 4],
                            bboxes[2 + j * 4],
                            bboxes[3 + j * 4],
                            i,
                            confidence};
                class_bbox_rects.push_back(c);
            }
        }
        std::sort(class_bbox_rects.begin(), class_bbox_rects.end(), cmp);

        // apply nms
        std::vector<Object> sorted_boxes;
        NmsSortedBboxes(class_bbox_rects,
                        nms_threshold,
                        std::min(top_k,
                                 static_cast<int>(class_bbox_rects.size())),
                        &sorted_boxes);
        // gather
        result->objects.insert(result->objects.end(), sorted_boxes.begin(),
                           sorted_boxes.end());
    }

    std::sort(result->objects.begin(), result->objects.end(), cmp);

    // output
    int num_detected = keep_top_k < static_cast<int>(result->objects.size()) ?
                       keep_top_k : static_cast<int>(result->objects.size());
    result->objects.resize(num_detected);

    return Status::OK();
}

Status ObjectDetection::Create(const ObjectDetectionContext &context,
                               ObjectDetection **object_detection_ptr) {
    *object_detection_ptr = new ObjectDetectionImpl(context);
    return Status::OK();
}

}  // namespace mace_kit
