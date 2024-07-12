#!/bin/bash

set -e

module reset
module use /soft/preview-modulefiles/24.086.0
module load oneapi/eng-compiler/2024.04.15.002
module load intel_compute_runtime/release/821.36
module use /soft/modulefiles/
module load cmake
module load git-lfs
module load ninja

# from Renzo on slack 02/01/2024
export IGC_FunctionCloningThreshold=1
export IGC_ControlInlineTinySize=100
export IGC_OCLInlineThreshold=200
export IGC_PartitionUnit=1
export IGC_ForceOCLSIMDWidth=16
export ZE_AFFINITY_MASK=0.0

export CC=`which icx`
export CXX=`which icpx`

srcdir=`dirname $0`
srcdir=`realpath $srcdir`
bindir=$srcdir-bin

mkdir -p $bindir

cmake -G Ninja -S $srcdir -B $bindir \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-fPIC -fp-model=precise -Wno-unused-command-line-argument -Wno-deprecated-declarations -fsycl-device-code-split=per_kernel -fsycl-max-parallel-link-jobs=128" \
  -DCMAKE_CXX_FLAGS_DEBUG="-g -O0 -fsycl-link-huge-device-code" \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_CXX_EXTENSIONS=OFF \
  -DBUILD_SHARED_LIBS=ON \
  -DKokkos_ENABLE_EXAMPLES=OFF \
  -DKokkos_ENABLE_TESTS=OFF \
  -DKokkos_ENABLE_SERIAL=ON \
  -DKokkos_ENABLE_SYCL=ON \
  -DKokkos_ARCH_INTEL_PVC=ON

cmake --build $bindir
