# cmake_minimum_required(VERSION 3.13)  # CMake version check
# project(simple_example)               # Create project "simple_example"
# set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard

# # Add main.cpp file of project root directory as source file
# set(SOURCE_FILES main.cpp Parser.hpp Parser.cpp Lexer.hpp Lexer.cpp ParserTests.cpp)

# # Add executable target with source files listed in SOURCE_FILES variable
# add_executable(ptx-parser ${SOURCE_FILES})

set(SRC
  Lexer.cpp
  Parser.cpp
)
add_library(PARSER ${SRC})
target_include_directories(PARSER
  PUBLIC
  ${COMPILER_SOURCE_DIR}/include
)