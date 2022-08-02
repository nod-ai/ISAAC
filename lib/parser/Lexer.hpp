#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ptx_parser {

enum Token_Type {
  OPCODE_ADD,
  OPCODE_SUB,
  TOKEN_COMMA,
  TOKEN_SEMICOLON,
  TOKEN_U8,
  TOKEN_U16,
  TOKEN_U32,
  TOKEN_U64,
  TOKEN_S8,
  TOKEN_S16,
  TOKEN_S32,
  TOKEN_S64,
  TOKEN_B8,
  TOKEN_B16,
  TOKEN_B32,
  TOKEN_B64,
  TOKEN_F16,
  TOKEN_F32,
  TOKEN_F64,
  TOKEN_IDENTIFIER,
  TOKEN_VERSION,
  TOKEN_DECIMAL_CONSTANT,
  TOKEN_DOUBLE_CONSTANT,
  TOKEN_UNRECOGNIZED,
  TOKEN_OPENBRACE,
  TOKEN_CLOSEBRACE,
  TOKRN_REG,
  TOKEN_LOC,
  TOKEN_LABEL,
  TOKEN_PRAGMA,
  TOKEN_STRING,
  TOKEN_CALL_PROTOTYPE,
  TOKEN_CALL_TARGETS,
  TOKEN_CARRY,
  TOKEN_UNSIGNED_DECIMAL_CONSTANT,
  TOKEN_SINGLE_CONSTANT,
  TOKEN_EXCLAMATION_MARK,
};

class Token {
public:
  enum Token_Type token_type;
  std::string value;
  std::string text;

  Token();
  Token(std::string text);
  void clear();
  std::string repr();
};

class Lexer {
  std::vector<std::string> text;

public:
  std::vector<std::string> get_symbols(std::string file_path_);
  std::vector<Token> parse(std::vector<std::string> symbols);
};

} // namespace ptx_parser
