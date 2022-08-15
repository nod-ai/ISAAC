set(SRC
  Lexer.cpp
  Parser.cpp
  ParserTests.cpp
)
add_library(PARSER ${SRC})
target_include_directories(PARSER
  PUBLIC
  ${COMPILER_SOURCE_DIR}/include
)