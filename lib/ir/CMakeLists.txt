set(SRC
  PTXIR.cpp
)
add_library(PTXIR ${SRC})
target_include_directories(PTXIR
  PUBLIC
  ${COMPILER_SOURCE_DIR}/include
)
