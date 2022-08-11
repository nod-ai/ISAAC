#pragma once

#include "Lexer.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace ptx {

class Node {
  std::vector<Node> children;
  Token value;

public:
  enum Node_Type node_type;

  void set_type(enum Node_Type type_) { node_type = type_; }

  void set_token(Token token_) { value = token_; }

  void add(Node child) { children.push_back(child); }

  void reset() {
    children.clear();
    node_type = noneNode;
  }

  std::string repr();
};

class Parser {

  int current_token_index;
  Node parse_tree;

public:
  void parse(std::vector<Token> tokens);
  void run_tests();

private:
  void advance(std::vector<Token> tokens);

  bool parse_grammer_sequence(std::vector<Node_Type> node_sequence,
                              std::vector<Token> tokens, Node &node);

  bool handle_token(Node_Type node_type, std::vector<Token> tokens, Node &node,
                    Node &parent_node);
  bool handle_grammer(Node_Type node_type, std::vector<Token> tokens,
                      Node &node, Node &parent_node);

  bool parse_statement(std::vector<Token> tokens, Node &node);

  // Statements

  bool parse_initializableDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_nonEntryStatement(std::vector<Token> tokens, Node &node);
  bool parse_entry(std::vector<Token> tokens, Node &node);
  bool parse_functionBody(std::vector<Token> tokens, Node &node);
  bool parse_functionDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_version(std::vector<Token> tokens, Node &node);
  bool parse_target(std::vector<Token> tokens, Node &node);
  bool parse_registerDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_fileDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_preprocessor(std::vector<Token> tokens, Node &node);
  bool parse_samplerDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_surfaceDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_textureDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_globalSharedDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_globalLocalDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_addressSize(std::vector<Token> tokens, Node &node);
  bool parse_entryDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_openBrace(std::vector<Token> tokens, Node &node);
  bool parse_closeBrace(std::vector<Token> tokens, Node &node);
  bool parse_entryStatements(std::vector<Token> tokens, Node &node);
  bool parse_entryStatement(std::vector<Token> tokens, Node &node);
  bool parse_location(std::vector<Token> tokens, Node &node);
  bool parse_label(std::vector<Token> tokens, Node &node);
  bool parse_optionalMetadata(std::vector<Token> tokens, Node &node);
  bool parse_pragma(std::vector<Token> tokens, Node &node);
  bool parse_callprototype(std::vector<Token> tokens, Node &node);
  bool parse_calltargets(std::vector<Token> tokens, Node &node);
  bool parse_returnTypeList(std::vector<Token> tokens, Node &node);
  bool parse_identifier(std::vector<Token> tokens, Node &node);
  bool parse_argumentTypeList(std::vector<Token> tokens, Node &node);
  bool parse_identifierList(std::vector<Token> tokens, Node &node);
  bool parse_instruction(std::vector<Token> tokens, Node &node);
  bool parse_registerPrefix(std::vector<Token> tokens, Node &node);
  bool parse_registerIdentifierList(std::vector<Token> tokens, Node &node);
  bool parse_uninitializable(std::vector<Token> tokens, Node &node);
  bool parse_addressableVariablePrefix(std::vector<Token> tokens, Node &node);
  bool parse_uninitializableDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_arrayDimensions(std::vector<Token> tokens, Node &node);
  bool parse_completeEntryStatement(std::vector<Token> tokens, Node &node);
  bool parse_guard(std::vector<Token> tokens, Node &node);
  bool parse_entryName(std::vector<Token> tokens, Node &node);
  bool parse_optionalArgumentList(std::vector<Token> tokens, Node &node);
  bool parse_performanceDirectives(std::vector<Token> tokens, Node &node);
  bool parse_ftzInstruction2(std::vector<Token> tokens, Node &node);
  bool parse_ftzInstruction3(std::vector<Token> tokens, Node &node);
  bool parse_approxInstruction2(std::vector<Token> tokens, Node &node);
  bool parse_basicInstruction3(std::vector<Token> tokens, Node &node);
  bool parse_bfe(std::vector<Token> tokens, Node &node);
  bool parse_bfi(std::vector<Token> tokens, Node &node);
  bool parse_bfind(std::vector<Token> tokens, Node &node);
  bool parse_brev(std::vector<Token> tokens, Node &node);
  bool parse_branch(std::vector<Token> tokens, Node &node);
  bool parse_addOrSub(std::vector<Token> tokens, Node &node);
  bool parse_addCOrSubC(std::vector<Token> tokens, Node &node);
  bool parse_atom(std::vector<Token> tokens, Node &node);
  bool parse_bar(std::vector<Token> tokens, Node &node);
  bool parse_barrier(std::vector<Token> tokens, Node &node);
  bool parse_brkpt(std::vector<Token> tokens, Node &node);
  bool parse_clz(std::vector<Token> tokens, Node &node);
  bool parse_cvt(std::vector<Token> tokens, Node &node);
  bool parse_cvta(std::vector<Token> tokens, Node &node);
  bool parse_isspacep(std::vector<Token> tokens, Node &node);
  bool parse_div(std::vector<Token> tokens, Node &node);
  bool parse_exit(std::vector<Token> tokens, Node &node);
  bool parse_ld(std::vector<Token> tokens, Node &node);
  bool parse_ldu(std::vector<Token> tokens, Node &node);
  bool parse_mad(std::vector<Token> tokens, Node &node);
  bool parse_mad24(std::vector<Token> tokens, Node &node);
  bool parse_madc(std::vector<Token> tokens, Node &node);
  bool parse_membar(std::vector<Token> tokens, Node &node);
  bool parse_mov(std::vector<Token> tokens, Node &node);
  bool parse_mul24(std::vector<Token> tokens, Node &node);
  bool parse_mul(std::vector<Token> tokens, Node &node);
  bool parse_notInstruction(std::vector<Token> tokens, Node &node);
  bool parse_pmevent(std::vector<Token> tokens, Node &node);
  bool parse_popc(std::vector<Token> tokens, Node &node);
  bool parse_prefetch(std::vector<Token> tokens, Node &node);
  bool parse_prefetchu(std::vector<Token> tokens, Node &node);
  bool parse_prmt(std::vector<Token> tokens, Node &node);
  bool parse_rcpSqrtInstructio(std::vector<Token> tokens, Node &node);
  bool parse_red(std::vector<Token> tokens, Node &node);
  bool parse_ret(std::vector<Token> tokens, Node &node);
  bool parse_sad(std::vector<Token> tokens, Node &node);
  bool parse_selp(std::vector<Token> tokens, Node &node);
  bool parse_set(std::vector<Token> tokens, Node &node);
  bool parse_setp(std::vector<Token> tokens, Node &node);
  bool parse_slct(std::vector<Token> tokens, Node &node);
  bool parse_st(std::vector<Token> tokens, Node &node);
  bool parse_suld(std::vector<Token> tokens, Node &node);
  bool parse_suq(std::vector<Token> tokens, Node &node);
  bool parse_sured(std::vector<Token> tokens, Node &node);
  bool parse_sust(std::vector<Token> tokens, Node &node);
  bool parse_testp(std::vector<Token> tokens, Node &node);
  bool parse_tex(std::vector<Token> tokens, Node &node);
  bool parse_tld4(std::vector<Token> tokens, Node &node);
  bool parse_trap(std::vector<Token> tokens, Node &node);
  bool parse_txq(std::vector<Token> tokens, Node &node);
  bool parse_vote(std::vector<Token> tokens, Node &node);
  bool parse_shfl(std::vector<Token> tokens, Node &node);
  bool parse_addOrSubOpcode(std::vector<Token> tokens, Node &node);
  bool parse_addModifier(std::vector<Token> tokens, Node &node);
  bool parse_dataType(std::vector<Token> tokens, Node &node);
  bool parse_operand(std::vector<Token> tokens, Node &node);
  bool parse_optionalFloatingRoundNumber(std::vector<Token> tokens, Node &node);
  bool parse_optionalFtz(std::vector<Token> tokens, Node &node);
  bool parse_optionalSaturate(std::vector<Token> tokens, Node &node);
  bool parse_constantOperand(std::vector<Token> tokens, Node &node);
  bool parse_nonLabelOperand(std::vector<Token> tokens, Node &node);
  bool parse_optionalVectorIndex(std::vector<Token> tokens, Node &node);
  bool parse_opcode(std::vector<Token> tokens, Node &node);
  bool parse_floatRoundingToken(std::vector<Token> tokens, Node &node);
  bool parse_targetElementList(std::vector<Token> tokens, Node &node);
  bool parse_targetElement(std::vector<Token> tokens, Node &node);
  bool parse_externOrVisible(std::vector<Token> tokens, Node &node);
  bool parse_uninitializableAddress(std::vector<Token> tokens, Node &node);
  bool parse_initializableAddress(std::vector<Token> tokens, Node &node);
  bool parse_initializable(std::vector<Token> tokens, Node &node);
  bool parse_arrayDimensionSet(std::vector<Token> tokens, Node &node);
  bool parse_functionBegin(std::vector<Token> tokens, Node &node);
  bool parse_functionBodyDefinition(std::vector<Token> tokens, Node &node);
  bool parse_optionalReturnArgument(std::vector<Token> tokens, Node &node);
  bool parse_functionName(std::vector<Token> tokens, Node &node);
  bool parse_argumentList(std::vector<Token> tokens, Node &node);
  bool parse_optionalSemicolon(std::vector<Token> tokens, Node &node);
  bool parse_argumentListBegin(std::vector<Token> tokens, Node &node);
  bool parse_argumentListBody(std::vector<Token> tokens, Node &node);
  bool parse_argumentListEnd(std::vector<Token> tokens, Node &node);
  bool parse_argumentDeclaration(std::vector<Token> tokens, Node &node);
  bool parse_parameter(std::vector<Token> tokens, Node &node);
  bool parse_alignment(std::vector<Token> tokens, Node &node);
  bool parse_statementVectorType(std::vector<Token> tokens, Node &node);
  bool parse_vectorIndex(std::vector<Token> tokens, Node &node);
};
} // namespace ptx