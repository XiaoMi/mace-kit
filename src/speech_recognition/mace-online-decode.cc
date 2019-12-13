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

#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <string>

#include "include/kaldi-mace.h"

struct Args_t {
  std::string configure_file; /* -c option*/
  std::string fst_file; /* -f option */
  std::string wav_file; /* -w option */
  std::string prediction_file; /* -o option */
  std::string vlog_level; /* -v op */
};

static void show_usage() {
    const char *usage =
        "Reads in wav file(s) and simulates online decoding with neural nets\n"
            "(nnet3 setup with MaceEngine),"
            " with optional iVector-based speaker adaptation and\n"
            "optional endpointing."
            "Note: some configuration values and inputs are\n"
            "set via config files whose filenames are passed as options\n"
            "\n"
            "Usage: mace-online-decode \n"
            " -c, --conf: required, path to configuration file.\n"
            " -f, --fst: required, path to fst file.\n"
            " -w, --wav: required, path to input wave file.\n"
            " -o, --out: required,"
            " path to save output predictions, in text mode.\n"
            " -v, --vlog: optional, enable log for debug if set to (1~5),"
            " default is zero.\n";

    std::cerr << usage << std::endl;
}

bool check_args(Args_t input_args) {
    return !input_args.prediction_file.empty() &&
        !input_args.wav_file.empty() &&
        !input_args.fst_file.empty() &&
        !input_args.configure_file.empty();
}


int main(int argc, char *argv[]) {
    Args_t input_args;
    input_args.vlog_level = "";
    input_args.configure_file = "";
    input_args.fst_file = "";
    input_args.wav_file = "";
    input_args.prediction_file = "";
    int opt;
    int option_index = 0;
    static const char *optString = "hc:f:w:o:v::";
    static struct option longOptions[] = {
        {"conf", required_argument, nullptr, 'c'},
        {"fst",  required_argument, nullptr, 'f'},
        {"wav", required_argument, nullptr, 'w'},
        {"out", required_argument, nullptr, 'o'},
        {"vlog", optional_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {0, 0, 0, 0}
    };

    while ( (opt = getopt_long(argc, argv,
                               optString,
                               longOptions,
                               &option_index)) != -1) {
        switch (opt) {
            case 'c':
                input_args.configure_file = optarg;
            break;
            case 'f':
                input_args.fst_file = optarg;
            break;
            case 'w':
                input_args.wav_file = optarg;
            break;
            case 'o':
                input_args.prediction_file = optarg;
            break;
            case 'v':
                input_args.vlog_level = optarg;
            break;
            default:
                show_usage();
            return -1;
        }
    }

    if (check_args(input_args)) {
        std::string result = kaldi::MaceNnet3WavDecode(
            input_args.configure_file,
            input_args.fst_file,
            input_args.wav_file,
            input_args.prediction_file,
            input_args.vlog_level);
      std::cout << "predictions:" << result << std::endl;
      return 0;
    } else {
        show_usage();
        return -1;
    }
}  // main()
