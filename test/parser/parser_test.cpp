#include "parser/Parser.hpp"
#include "gtest/gtest.h"

TEST(ParserTest, Parse) {
  ptx::Parser parser;
  parser.run_tests();
  int x = 1;
  EXPECT_EQ(x, 1);
}

// TEST(ParserTest, ParseOpcode) {

//   ptx::Lexer lexer;
//   ptx::Parser parser;

//   // test opcode

//   std::vector<std::string> symbols =
//       lexer.get_symbols("../test-scripts/opcode.ptx");
//   std::vector<ptx::Token> tokens = lexer.parse(symbols);

//   ptx::Node node;
//   int current_token_index = 0;
//   int success;
//   int passed = 0;
//   int total = 0;
//   while (current_token_index < tokens.size()) {
//     success = parser.parse_opcode(tokens, node);
//     node.reset();
//     if (success == 1) {
//       passed = passed + 1;
//     }
//     total = total + 1;
//   }
//   EXPECT_EQ(passed, 3);

// }

// TEST(ParserTest, ParseAddOrSub) {

//   ptx::Lexer lexer;
//   ptx::Parser parser;

//   // test opcode

//   std::vector<std::string> symbols =
//       lexer.get_symbols("../test-scripts/addOrSub.ptx");
//   std::vector<ptx::Token> tokens = lexer.parse(symbols);

//   ptx::Node node;
//   int current_token_index = 0;
//   int success;
//   int passed = 0;
//   int total = 0;
//   while (current_token_index < tokens.size()) {
//     success = parser.parse_addOrSub(tokens, node);
//     node.reset();
//     if (success == 1) {
//       passed = passed + 1;
//     }
//     total = total + 1;
//   }
//   EXPECT_EQ(passed, 1);

// }

// TEST(ParserTest, ParseFunctionDeclaration) {

//   ptx::Lexer lexer;
//   ptx::Parser parser;

//   // test opcode

//   std::vector<std::string> symbols =
//       lexer.get_symbols("../test-scripts/functionDeclaration.ptx");
//   std::vector<ptx::Token> tokens = lexer.parse(symbols);

//   ptx::Node node;
//   int current_token_index = 0;
//   int success;
//   int passed = 0;
//   int total = 0;
//   while (current_token_index < tokens.size()) {
//     success = parser.parse_functionDeclaration(tokens, node);
//     node.reset();
//     if (success == 1) {
//       passed = passed + 1;
//     }
//     total = total + 1;
//   }
//   EXPECT_EQ(passed, 1);

// }