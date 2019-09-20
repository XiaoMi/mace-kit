#!/usr/bin/env sh

set -e

# build for host
BUILD_DIR=cmake-build/host
mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR}
cmake -DCMAKE_INSTALL_PREFIX=install \
      ../..
make -j6 VERBOSE=1 && make install
cd ../..
