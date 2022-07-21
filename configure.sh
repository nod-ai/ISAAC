#!/bin/bash

set -e

cmake -GNinja -Bbuild \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DLLVM_ENABLE_PROJECTS=mlir \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DLLVM_EXTERNAL_PROJECTS=compiler \
    -DLLVM_EXTERNAL_COMPILER_SOURCE_DIR=`pwd` \
    -DMLIR_ENABLE_BINDINGS_PYTHON=OFF \
    -DLLVM_TARGETS_TO_BUILD="X86;NVPTX;AMDGPU" \
    external/llvm-project/llvm
