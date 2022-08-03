#pragma once

#include "Lexer.hpp"
#include <string>
#include <vector>

namespace ptx_parser {

enum Node_Type {
  // empty node type
  noneNode,
  // leaf modes
  leaf,
  // grammer nodes
  statement,
  initializableDeclaration,
  nonEntryStatement,
  version,
  closeBrace,
  openBrace,
  entry,
  functionBody,
  functionDeclaration,
  entryDeclaration,
  entryStatements,
  completeEntryStatement,
  uninitializableDeclaration,
  entryStatement,
  registerDeclaration,
  location,
  pragma,
  callprototype,
  calltargets,
  instruction,
  addOrSub,
  addModifier,
  addOrSubOpcode,
  dataType,
  operand,
  optionalFloatingRoundNumber,
  optionalFtz,
  optionalSaturate,
  identifier,
  opcode,
  floatingRoundingToken,
  entryName,
  optionalArgumentList,
  performanceDirectives,
  optionalMetadata,
  guard,
  uninitializable,
  arrayDimensions,
  returnTypeList,
  addressableVariablePrefix,
  registerPrefix,
  argumentTypeList,
  registerIdentifierList,
  identifierList,
  optionalVectorIndex,
  target,
  targetElementList,
  targetElement,
  externOrVisible,
};

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

  int parse_statement(std::vector<Token> tokens, Node &node);

  // Statements

  int parse_initializableDeclaration(std::vector<Token> tokens, Node &node);
  int parse_nonEntryStatement(std::vector<Token> tokens, Node &node);
  int parse_entry(std::vector<Token> tokens, Node &node);
  int parse_functionBody(std::vector<Token> tokens, Node &node);
  int parse_functionDeclaration(std::vector<Token> tokens, Node &node);
  int parse_version(std::vector<Token> tokens, Node &node);
  int parse_target(std::vector<Token> tokens, Node &node);
  int parse_registerDeclaration(std::vector<Token> tokens, Node &node);
  int parse_fileDeclaration(std::vector<Token> tokens, Node &node);
  int parse_preprocessor(std::vector<Token> tokens, Node &node);
  int parse_samplerDeclaration(std::vector<Token> tokens, Node &node);
  int parse_surfaceDeclaration(std::vector<Token> tokens, Node &node);
  int parse_textureDeclaration(std::vector<Token> tokens, Node &node);
  int parse_globalSharedDeclaration(std::vector<Token> tokens, Node &node);
  int parse_globalLocalDeclaration(std::vector<Token> tokens, Node &node);
  int parse_addressSize(std::vector<Token> tokens, Node &node);
  int parse_entryDeclaration(std::vector<Token> tokens, Node &node);
  int parse_openBrace(std::vector<Token> tokens, Node &node);
  int parse_closeBrace(std::vector<Token> tokens, Node &node);
  int parse_entryStatements(std::vector<Token> tokens, Node &node);
  int parse_entryStatement(std::vector<Token> tokens, Node &node);
  int parse_location(std::vector<Token> tokens, Node &node);
  int parse_label(std::vector<Token> tokens, Node &node);
  int parse_optionalMetadata(std::vector<Token> tokens, Node &node);
  int parse_pragma(std::vector<Token> tokens, Node &node);
  int parse_callprototype(std::vector<Token> tokens, Node &node);
  int parse_calltargets(std::vector<Token> tokens, Node &node);
  int parse_returnTypeList(std::vector<Token> tokens, Node &node);
  int parse_identifier(std::vector<Token> tokens, Node &node);
  int parse_argumentTypeList(std::vector<Token> tokens, Node &node);
  int parse_identifierList(std::vector<Token> tokens, Node &node);
  int parse_instruction(std::vector<Token> tokens, Node &node);
  int parse_registerPrefix(std::vector<Token> tokens, Node &node);
  int parse_registerIdentifierList(std::vector<Token> tokens, Node &node);
  int parse_uninitializable(std::vector<Token> tokens, Node &node);
  int parse_addressableVariablePrefix(std::vector<Token> tokens, Node &node);
  int parse_uninitializableDeclaration(std::vector<Token> tokens, Node &node);
  int parse_arrayDimensions(std::vector<Token> tokens, Node &node);
  int parse_completeEntryStatement(std::vector<Token> tokens, Node &node);
  int parse_guard(std::vector<Token> tokens, Node &node);
  int parse_entryName(std::vector<Token> tokens, Node &node);
  int parse_optionalArgumentList(std::vector<Token> tokens, Node &node);
  int parse_performanceDirectives(std::vector<Token> tokens, Node &node);
  int parse_ftzInstruction2(std::vector<Token> tokens, Node &node);
  int parse_ftzInstruction3(std::vector<Token> tokens, Node &node);
  int parse_approxInstruction2(std::vector<Token> tokens, Node &node);
  int parse_basicInstruction3(std::vector<Token> tokens, Node &node);
  int parse_bfe(std::vector<Token> tokens, Node &node);
  int parse_bfi(std::vector<Token> tokens, Node &node);
  int parse_bfind(std::vector<Token> tokens, Node &node);
  int parse_brev(std::vector<Token> tokens, Node &node);
  int parse_branch(std::vector<Token> tokens, Node &node);
  int parse_addOrSub(std::vector<Token> tokens, Node &node);
  int parse_addCOrSubC(std::vector<Token> tokens, Node &node);
  int parse_atom(std::vector<Token> tokens, Node &node);
  int parse_bar(std::vector<Token> tokens, Node &node);
  int parse_barrier(std::vector<Token> tokens, Node &node);
  int parse_brkpt(std::vector<Token> tokens, Node &node);
  int parse_clz(std::vector<Token> tokens, Node &node);
  int parse_cvt(std::vector<Token> tokens, Node &node);
  int parse_cvta(std::vector<Token> tokens, Node &node);
  int parse_isspacep(std::vector<Token> tokens, Node &node);
  int parse_div(std::vector<Token> tokens, Node &node);
  int parse_exit(std::vector<Token> tokens, Node &node);
  int parse_ld(std::vector<Token> tokens, Node &node);
  int parse_ldu(std::vector<Token> tokens, Node &node);
  int parse_mad(std::vector<Token> tokens, Node &node);
  int parse_mad24(std::vector<Token> tokens, Node &node);
  int parse_madc(std::vector<Token> tokens, Node &node);
  int parse_membar(std::vector<Token> tokens, Node &node);
  int parse_mov(std::vector<Token> tokens, Node &node);
  int parse_mul24(std::vector<Token> tokens, Node &node);
  int parse_mul(std::vector<Token> tokens, Node &node);
  int parse_notInstruction(std::vector<Token> tokens, Node &node);
  int parse_pmevent(std::vector<Token> tokens, Node &node);
  int parse_popc(std::vector<Token> tokens, Node &node);
  int parse_prefetch(std::vector<Token> tokens, Node &node);
  int parse_prefetchu(std::vector<Token> tokens, Node &node);
  int parse_prmt(std::vector<Token> tokens, Node &node);
  int parse_rcpSqrtInstructio(std::vector<Token> tokens, Node &node);
  int parse_red(std::vector<Token> tokens, Node &node);
  int parse_ret(std::vector<Token> tokens, Node &node);
  int parse_sad(std::vector<Token> tokens, Node &node);
  int parse_selp(std::vector<Token> tokens, Node &node);
  int parse_set(std::vector<Token> tokens, Node &node);
  int parse_setp(std::vector<Token> tokens, Node &node);
  int parse_slct(std::vector<Token> tokens, Node &node);
  int parse_st(std::vector<Token> tokens, Node &node);
  int parse_suld(std::vector<Token> tokens, Node &node);
  int parse_suq(std::vector<Token> tokens, Node &node);
  int parse_sured(std::vector<Token> tokens, Node &node);
  int parse_sust(std::vector<Token> tokens, Node &node);
  int parse_testp(std::vector<Token> tokens, Node &node);
  int parse_tex(std::vector<Token> tokens, Node &node);
  int parse_tld4(std::vector<Token> tokens, Node &node);
  int parse_trap(std::vector<Token> tokens, Node &node);
  int parse_txq(std::vector<Token> tokens, Node &node);
  int parse_vote(std::vector<Token> tokens, Node &node);
  int parse_shfl(std::vector<Token> tokens, Node &node);
  int parse_addOrSubOpcode(std::vector<Token> tokens, Node &node);
  int parse_addModifier(std::vector<Token> tokens, Node &node);
  int parse_dataType(std::vector<Token> tokens, Node &node);
  int parse_operand(std::vector<Token> tokens, Node &node);
  int parse_optionalFloatingRoundNumber(std::vector<Token> tokens, Node &node);
  int parse_optionalFtz(std::vector<Token> tokens, Node &node);
  int parse_optionalSaturate(std::vector<Token> tokens, Node &node);
  int parse_constantOperand(std::vector<Token> tokens, Node &node);
  int parse_nonLabelOperand(std::vector<Token> tokens, Node &node);
  int parse_optionalVectorIndex(std::vector<Token> tokens, Node &node);
  int parse_opcode(std::vector<Token> tokens, Node &node);
  int parse_floatRoundingToken(std::vector<Token> tokens, Node &node);
  int parse_targetElementList(std::vector<Token> tokens, Node &node);
  int parse_targetElement(std::vector<Token> tokens, Node &node);
  int parse_externOrVisible(std::vector<Token> tokens, Node &node);
};
} // namespace ptx_parser