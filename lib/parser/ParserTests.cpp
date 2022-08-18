#include "parser/Parser.hpp"
namespace ptx {

void Parser::run_tests() {

  ptx::Lexer lexer;

  // test opcode

  std::vector<std::string> symbols =
      lexer.get_symbols("../test-scripts/opcode.ptx");
  std::vector<ptx::Token> tokens = lexer.parse(symbols);

  ptx::Node node;
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
    success = parse_entryStatements(tokens, node);
    if (success == 1) {
      std::cout << node.repr() << std::endl;
      passed = passed + 1;
    }
    total = total + 1;
    node.reset();
  }
  std::cout << "ADDORSUB: Passed " << std::to_string(passed) << "/"
            << std::to_string(total) << " tests" << std::endl;

  // test functionDeclaration

  symbols = lexer.get_symbols("../test-scripts/functionDeclaration.ptx");
  tokens = lexer.parse(symbols);

  node.reset();
  current_token_index = 0;
  passed = 0;
  total = 0;
  while (current_token_index < tokens.size()) {
    success = parse_functionDeclaration(tokens, node);

    if (success == 1) {
      std::cout << node.repr() << std::endl;
      passed = passed + 1;
    }
    total = total + 1;
    node.reset();
  }
  std::cout << "FUNCTIONDECLARATION: Passed " << std::to_string(passed) << "/"
            << std::to_string(total) << " tests" << std::endl;

  // test functionBodyDefinition

  symbols = lexer.get_symbols("../test-scripts/functionDeclaration.ptx");
  tokens = lexer.parse(symbols);

  node.reset();
  current_token_index = 0;
  passed = 0;
  total = 0;
  while (current_token_index < tokens.size()) {
    success = parse_functionBodyDefinition(tokens, node);

    if (success == 1) {
      std::cout << node.repr() << std::endl;
      passed = passed + 1;
    }
    total = total + 1;
    node.reset();
  }
  std::cout << "FUNCTIONBODYDEFINITION: Passed " << std::to_string(passed)
            << "/" << std::to_string(total) << " tests" << std::endl;

  // test initilaizableDeclaration

  symbols = lexer.get_symbols("../test-scripts/initializableDeclaration.ptx");
  tokens = lexer.parse(symbols);

  node.reset();
  current_token_index = 0;
  passed = 0;
  total = 0;
  while (current_token_index < tokens.size()) {
    success = parse_initializableDeclaration(tokens, node);
    if (!success) {
      break;
    }

    if (success == 1) {
      std::cout << node.repr() << std::endl;
      passed = passed + 1;
    }
    total = total + 1;
    node.reset();
  }
  std::cout << "INITIALIZABLEDECLARATION: Passed " << std::to_string(passed)
            << "/" << std::to_string(total) << " tests" << std::endl;

  // test functionBody

  symbols = lexer.get_symbols("../test-scripts/function.ptx");
  tokens = lexer.parse(symbols);

  node.reset();
  current_token_index = 0;
  passed = 0;
  total = 0;
  while (current_token_index < tokens.size()) {
    success = parse_statement(tokens, node);
    if (!success) {
      break;
    }

    if (success == 1) {
      std::cout << node.repr() << std::endl;
      passed = passed + 1;
    }
    total = total + 1;
    node.reset();
  }
  std::cout << "FUNCTIONBODY: Passed " << std::to_string(passed) << "/"
            << std::to_string(total) << " tests" << std::endl;
}

} // namespace ptx
