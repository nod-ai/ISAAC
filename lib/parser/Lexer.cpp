#include "Lexer.hpp"
#include <regex>

namespace ptx_parser {

void Token::clear() {
  token_type = TOKEN_UNRECOGNIZED;
  value.erase();
}

Token::Token(std::string text_) {
  std::__cxx11::regex float_e("[0-9]*\\.[0-9]{1,}");
  std::__cxx11::regex int_e("[0-9]");
  std::__cxx11::regex identifier_e("%[0-9a-zA-Z]+");
  text = text_;
  if (text_ == ".version") {
    token_type = TOKEN_VERSION;
  } else if (std::regex_match(text_, float_e)) {
    std::cout << text_ << std::endl;
    token_type = TOKEN_DOUBLE_CONSTANT;
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
    std::__cxx11::regex e("([,;-\\[\\](){}\\ \\t]|[^,;-\\[\\](){}\\ \\t]+)");
    if (line != "") {
      std::regex_iterator<std::string::iterator> rit(line.begin(), line.end(),
                                                     e);
      std::regex_iterator<std::string::iterator> rend;
      while (rit != rend) {
        if (rit->str() == "//") {
          break;
        } else if (rit->str() != " ") {
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
