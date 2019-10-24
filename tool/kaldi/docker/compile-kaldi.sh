#!/bin/bash

KALDI_ROOT=${WORKING_DIR}/kaldi
cd ${KALDI_ROOT}/tools

# tools directory --> we'll only compile OpenFST
OPENFST_VERSION=$(grep -oP "OPENFST_VERSION *\?= *\K(.*)$" ${WORKING_DIR}/kaldi/tools/Makefile)
wget -T 10 -t 1 http://openfst.cs.nyu.edu/twiki/pub/FST/FstDownload/openfst-${OPENFST_VERSION}.tar.gz
tar -zxvf openfst-${OPENFST_VERSION}.tar.gz
OPENFST_DIR=${KALDI_ROOT}/tools/openfst-${OPENFST_VERSION}
  cd ${OPENFST_DIR}
  CXX=clang++ ./configure --prefix=${OPENFST_DIR} --enable-static --enable-shared --enable-far --enable-ngram-fsts --host=arm-linux-androideabi LIBS="-ldl"
  make clean
  make -j 6
  make install

  cd ..
  ln -s openfst-${OPENFST_VERSION} openfst
  cd ../tools
  make cub
  make install

#  ## source directory
  cd ${KALDI_ROOT}/src
  MACELIBDIR=${KALDI_ROOT}/tools/libmace/
  MACEINCLUDE=${KALDI_ROOT}/tools/libmace/include/
  CXX=clang++ ./configure --use-cuda=no --shared --mace-libdir=${MACELIBDIR} --mace-incdir=${MACEINCLUDE} --fst-root=${OPENFST_DIR} --android-incdir=${ANDROID_TOOLCHAIN_PATH}/sysroot/usr/include/ --host=arm-linux-androideabi --openblas-root=${WORKING_DIR}/OpenBLAS/install
  sed -i 's/-g # -O3 -DKALDI_PARANOID/-O3 -DNDEBUG/g' kaldi.mk
  make -j clean depend
  make -j 6
  cd ${KALDI_ROOT}/src/macebin
  make -j clean depend
  make -j 6
