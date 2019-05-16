// Copyright 2018 The MACE Authors. All Rights Reserved.
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

// This is a generated file. DO NOT EDIT!

#ifndef MACE_CODEGEN_MODELS_SSD_MOBILENET_V2_SSD_MOBILENET_V2_H_
#define MACE_CODEGEN_MODELS_SSD_MOBILENET_V2_SSD_MOBILENET_V2_H_

#include <string>

#include "mace/public/mace.h"

namespace mace {
namespace ssd_mobilenet_v2 {


extern const unsigned char *LoadModelData();

extern const std::shared_ptr<NetDef> CreateNet();

extern const std::string ModelName();
extern const std::string ModelChecksum();
extern const std::string ModelBuildTime();
extern const std::string ModelBuildOptions();

}  // namespace ssd_mobilenet_v2
}  // namespace mace

#endif  // MACE_CODEGEN_MODELS_SSD_MOBILENET_V2_SSD_MOBILENET_V2_H_
