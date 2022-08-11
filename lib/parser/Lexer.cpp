#include "parser/Lexer.hpp"
#include <regex>

namespace ptx {

void Token::clear() {
  token_type = TOKEN_UNRECOGNIZED;
  value.erase();
}

Token::Token(std::string text_) {
  std::__cxx11::regex float_e("[0-9]*\\.[0-9]{1,}");
  std::__cxx11::regex int_e("[0-9]");
  std::__cxx11::regex identifier_e(
      "[[:alpha:]][[:alnum:]_$]*|[_$%][[:alnum:]_$]+");
  text = text_;
  if (text_ == ".version") {
    token_type = TOKEN_VERSION;
  } else if (text_ == ".weak") {
    token_type = TOKEN_WEAK;
  } else if (text_ == ".func") {
    token_type = TOKEN_FUNCTION;
  } else if (text_ == ".param") {
    token_type = TOKEN_PARAM;
  } else if (text_ == "b32") {
    token_type = TOKEN_B32;
  } else if (text_ == ".b64") {
    token_type = TOKEN_B64;
  } else if (text_ == "(") {
    token_type = TOKEN_OPENPARENTHESIS;
  } else if (text_ == ")") {
    token_type = TOKEN_CLOSEPARENTHESIS;
  } else if (text_ == "add") {
    token_type = OPCODE_ADD;
  } else if (text_ == "sub") {
    token_type = OPCODE_SUB;
  } else if (text_ == "addc") {
    token_type = OPCODE_ADDC;
  } else if (text_ == "atom") {
    token_type = OPCODE_ATOM;
  } else if (text_ == "abs") {
    token_type = OPCODE_ABS;
  } else if (text_ == "and") {
    token_type = OPCODE_AND;
  } else if (text_ == "bar") {
    token_type = OPCODE_BAR;
  } else if (text_ == "barrier") {
    token_type = OPCODE_BARRIER;
  } else if (text_ == "bfi") {
    token_type = OPCODE_BFI;
  } else if (text_ == "bfe") {
    token_type = OPCODE_BFE;
  } else if (text_ == "bfind") {
    token_type = OPCODE_BFIND;
  } else if (text_ == "bra") {
    token_type = OPCODE_BRA;
  } else if (text_ == "brev") {
    token_type = OPCODE_BREV;
  } else if (text_ == "brkpt") {
    token_type = OPCODE_BRKPT;
  } else if(text_ == "div") {
    token_type = OPCODE_DIV;
  } else if (text_ == ".cc") {
    token_type = TOKEN_CARRY;
  } else if (text_ == ".s8") {
    token_type = TOKEN_S8;
  } else if (text_ == ".s16") {
    token_type = TOKEN_S16;
  } else if (text_ == ".s32") {
    token_type = TOKEN_S32;
  } else if (text_ == ".s64") {
    token_type = TOKEN_S64;
  } else if (text_ == ".f16") {
    token_type = TOKEN_F16;
  } else if (text_ == ".f32") {
    token_type = TOKEN_F32;
  } else if (text_ == ".f64") {
    token_type = TOKEN_F64;
  } else if (text_ == ";") {
    token_type = TOKEN_SEMICOLON;
  } else if (text_ == ",") {
    token_type = TOKEN_COMMA;
  } else if (text_ == ".rn") {
    token_type = TOKEN_RN;
  } else if (std::regex_match(text_, float_e)) {
    token_type = TOKEN_DOUBLE_CONSTANT;
  } else if (std::regex_match(text_, identifier_e)) {
    token_type = TOKEN_IDENTIFIER;
  } else {
    token_type = TOKEN_UNRECOGNIZED;
  }
}

Token::Token() {}

std::vector<std::string> Lexer::get_symbols(std::string file_path) {
  text.clear();
  std::vector<std::string> symbols;
  std::string line;
  std::ifstream file_stream(file_path);

  if (!file_stream) {
    std::cout
        << "There was a problem opening the file. Press any key to close.\n";
    return symbols;
  }

  while (file_stream.good()) {
    getline(file_stream, line);
    std::__cxx11::regex e("\\d*\\.\\d+|[ ,;\t\\]\\[\\}\\{\\)\\(\n]|\\.{0,1}[^ "
                          ",;\t\\]\\[\\}\\{\\)\\(\n\\.]+");
    if (line != "") {
      std::regex_iterator<std::string::iterator> rit(line.begin(), line.end(),
                                                     e);
      std::regex_iterator<std::string::iterator> rend;
      while (rit != rend) {
        if (rit->str() == "//") {
          break;
        } else if (rit->str() != " " && rit->str() != "\n" &&
                   rit->str() != "\t") {
          symbols.push_back(rit->str());
        }
        ++rit;
      }
    }
  }
  return symbols;
}

std::vector<Token> Lexer::parse(std::vector<std::string> symbols) {
  std::vector<Token> tokens;
  for (std::string current_symbol : symbols) {
    Token token(current_symbol);
    tokens.push_back(token);
  }
  return tokens;
}

} // namespace ptx_parser