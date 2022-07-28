cmake_minimum_required(VERSION 3.23)
project(compiler LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0116)
  cmake_policy(SET CMP0116 OLD)
endif()

set(MLIR_MAIN_SRC_DIR ${LLVM_MAIN_SRC_DIR}/../mlir)
set(MLIR_INCLUDE_DIR ${LLVM_MAIN_SRC_DIR}/../mlir/include)
set(MLIR_GENERATED_INCLUDE_DIR ${LLVM_BINARY_DIR}/tools/mlir/include)
set(MLIR_INCLUDE_DIRS "${MLIR_INCLUDE_DIR};${MLIR_GENERATED_INCLUDE_DIR}")

set(VISA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(VISA_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(LLVM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/llvm-project/llvm")
set(COMPILER_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

list(APPEND CMAKE_MODULE_PATH ${MLIR_MAIN_SRC_DIR}/cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${LLVM_MAIN_SRC_DIR}/cmake)
set(MLIR_TABLEGEN_EXE mlir-tblgen)

include(TableGen)
include(AddLLVM)
include(AddMLIR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(AddGTest)

add_subdirectory(lib)

set(ENABLE_TESTS ON)
if(ENABLE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(external/googletest)
  add_subdirectory(test)
endif()
