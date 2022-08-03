#include "Parser.hpp"
namespace ptx_parser {

void Parser::run_tests() {

  ptx_parser::Lexer lexer;

  // test opcode

  std::vector<std::string> symbols =
      lexer.get_symbols("../test-scripts/opcode.ptx");
  std::vector<ptx_parser::Token> tokens = lexer.parse(symbols);

  ptx_parser::Node node;
  current_token_index = 0;
  int success;
  int passed = 0;
  int total = 0;
  while (current_token_index < tokens.size()) {
    success = parse_opcode(tokens, node);
    node.reset();
    if (success == 1) {
      passed = passed + 1;
    }
    total = total + 1;
  }
  std::cout << "OPCODE: Passed " << std::to_string(passed) << "/"
            << std::to_string(total) << " tests" << std::endl;

  // test addOrSub

  symbols = lexer.get_symbols("../test-scripts/addOrSub.ptx");
  tokens = lexer.parse(symbols);

  node.reset();
  current_token_index = 0;
  passed = 0;
  total = 0;
  while (current_token_index < tokens.size()) {
    success = parse_addOrSub(tokens, node);
    node.reset();
    if (success == 1) {
      passed = passed + 1;
    }
    total = total + 1;
  }
  std::cout << "ADDORSUB: Passed " << std::to_string(passed) << "/"
            << std::to_string(total) << " tests" << std::endl;
}

} // namespace ptx_parser
