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

#ifndef MACE_CODEGEN_ENGINE_MACE_ENGINE_FACTORY_H_
#define MACE_CODEGEN_ENGINE_MACE_ENGINE_FACTORY_H_
#include <map>
#include <memory>
#include <string>
#include <vector>

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

namespace {
std::map<std::string, int> model_name_map {
  std::make_pair("ssd_mobilenet_v2", 0),
};
}  // namespace

/// \brief Create MaceEngine from code
///
/// Create MaceEngine object based on model graph code and model data file or
/// model data code.
///
/// \param model_name[in]: the name of model you want to use.
/// \param model_data_file[in]: the path of model data file,
///        if model_data_format is code, just pass empty string("")
/// \param input_nodes[in]: the array of input nodes' name
/// \param output_nodes[in]: the array of output nodes' name
/// \param config[in]: configurations for MaceEngine.
/// \param engine[out]: output MaceEngine object
/// \return MaceStatus::MACE_SUCCESS for success, MACE_INVALID_ARGS for wrong arguments,
///         MACE_OUT_OF_RESOURCES for resources is out of range.
__attribute__((deprecated)) MaceStatus CreateMaceEngineFromCode(
    const std::string &model_name,
    const std::string &model_data_file,
    const std::vector<std::string> &input_nodes,
    const std::vector<std::string> &output_nodes,
    const MaceEngineConfig &config,
    std::shared_ptr<MaceEngine> *engine) {
  // load model
  if (engine == nullptr) {
    return MaceStatus::MACE_INVALID_ARGS;
  }
  std::shared_ptr<NetDef> net_def;
  (void)model_data_file;
  const unsigned char * model_data;
  MaceStatus status = MaceStatus::MACE_SUCCESS;
  switch (model_name_map[model_name]) {
    case 0:
      net_def = mace::ssd_mobilenet_v2::CreateNet();
      engine->reset(new mace::MaceEngine(config));
      model_data = mace::ssd_mobilenet_v2::LoadModelData();
      status = (*engine)->Init(net_def.get(), input_nodes, output_nodes,
                               model_data);
      break;
   default:
     status = MaceStatus::MACE_INVALID_ARGS;
  }

  return status;
}

MaceStatus CreateMaceEngineFromCode(
    const std::string &model_name,
    const unsigned char *model_weights_data,
    const size_t model_weights_data_size,
    const std::vector<std::string> &input_nodes,
    const std::vector<std::string> &output_nodes,
    const MaceEngineConfig &config,
    std::shared_ptr<MaceEngine> *engine) {
  // load model
  if (engine == nullptr) {
    return MaceStatus::MACE_INVALID_ARGS;
  }
  std::shared_ptr<NetDef> net_def;
  const unsigned char * model_data;
  (void)model_weights_data;
  // TODO(yejianwu) Add buffer range checking
  (void)model_weights_data_size;

  MaceStatus status = MaceStatus::MACE_SUCCESS;
  switch (model_name_map[model_name]) {
    case 0:
      net_def = mace::ssd_mobilenet_v2::CreateNet();
      engine->reset(new mace::MaceEngine(config));
      model_data = mace::ssd_mobilenet_v2::LoadModelData();
      status = (*engine)->Init(net_def.get(), input_nodes, output_nodes,
                               model_data);
      break;
   default:
     status = MaceStatus::MACE_INVALID_ARGS;
  }

  return status;
}

}  // namespace mace
#endif  // MACE_CODEGEN_ENGINE_MACE_ENGINE_FACTORY_H_