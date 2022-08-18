#include "parser/Parser.hpp"

namespace ptx {

std::string Node::repr() {
  std::string output = "";
  if (children.size() > 0) {
    output = output + "(";
  }
  if (node_type == leaf) {
    output = output + value.text;
  }
  if (children.size() > 0) {
    for (Node child : children) {
      output = output + child.repr();
    }
    output = output + ")";
  }
  return output;
}

void Parser::parse(std::vector<Token> tokens) {
  current_token_index = 0;
  bool success = false;
  Node child_node;

  while (current_token_index < tokens.size()) {

    success = parse_statement(tokens, child_node);

    if (success) {
      parse_tree.add(child_node);
      std::cout << "SUCCESS" << std::endl;
    } else {
      std::cout << "ERROR" << std::endl;
    }
  }
}

bool Parser::handle_token(Node_Type node_type, std::vector<Token> tokens, Node &node, Node &parent_node) {
  if(tokens[current_token_index].token_type == node_type) {
    node.set_type(leaf);
    node.set_token(tokens[current_token_index]);
    parent_node.add(node);
    advance(tokens);
    return true;
  }
  parent_node.reset();
  node.reset();
  return false;
}

bool Parser::parse_grammar_sequence(std::vector<Node_Type> node_sequence,
                                    std::vector<Token> tokens, Node &node) {
  Node child_node;
  int current_token_index_placeholder = current_token_index;
  bool success;

  for (Node_Type node_type : node_sequence) {
    child_node.reset();
    switch(node_type) {

      // Tokens

      case OPCODE_ADD:
        if( ! handle_token(OPCODE_ADD, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case OPCODE_SUB:
        if( ! handle_token(OPCODE_SUB, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case OPCODE_COS:
        if( ! handle_token(OPCODE_COS, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case OPCODE_SQRT:
        if( ! handle_token(OPCODE_SQRT, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case TOKEN_SEMICOLON:
        if (!handle_token(TOKEN_SEMICOLON, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case TOKEN_ALIGN:
        if (!handle_token(TOKEN_ALIGN, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case TOKEN_DECIMAL_CONSTANT:
        if (!handle_token(TOKEN_DECIMAL_CONSTANT, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case TOKEN_OPENBRACKET:
        if (!handle_token(TOKEN_OPENBRACKET, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

      case TOKEN_CLOSEBRACKET:
        if (!handle_token(TOKEN_CLOSEBRACKET, tokens, child_node, node)) {
          current_token_index = current_token_index_placeholder;
          return false;
        }
        break;

        // grammars

      case statement:
        success = parse_statement(tokens, child_node);
        if(success) {
          node.add(child_node);
        } else {
          current_token_index = current_token_index_placeholder;
          node.reset();
          child_node.reset();
          return false;
        }
        break;

        case initializableDeclaration:
        success = parse_initializableDeclaration(tokens, child_node);
        if(success) {
          node.add(child_node);
        } else {
          current_token_index = current_token_index_placeholder;
          node.reset();
          child_node.reset();
          return false;
        }
        break;

        case externOrVisible:
        success = parse_externOrVisible(tokens, child_node);
        if(success) {
          node.add(child_node);
        } else {
          current_token_index = current_token_index_placeholder;
          node.reset();
          child_node.reset();
          return false;
        }
        break;

        case initializableAddress:
        success = parse_initializableAddress(tokens, child_node);
        if(success) {
          node.add(child_node);
        } else {
          current_token_index = current_token_index_placeholder;
          node.reset();
          child_node.reset();
          return false;
        }
        break;

        case nonEntryStatement:
          success = parse_nonEntryStatement(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case entry:
          success = parse_entry(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case functionBody:
          success = parse_functionBody(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case functionDeclaration:
          success = parse_functionDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case version:
          success = parse_version(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case vectorIndex:
          success = parse_vectorIndex(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case target:
          success = parse_target(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case registerDeclaration:
          success = parse_registerDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case fileDeclaration:
          success = parse_fileDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case preprocessor:
          success = parse_preprocessor(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case samplerDeclaration:
          success = parse_samplerDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case surfaceDeclaration:
          success = parse_surfaceDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case textureDeclaration:
          success = parse_textureDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case globalSharedDeclaration:
          success = parse_globalSharedDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case globalLocalDeclaration:
          success = parse_globalLocalDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case addressSize:
          success = parse_addressSize(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case addressableVariablePrefix:
          success = parse_addressableVariablePrefix(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case entryDeclaration:
          success = parse_entryDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case openBrace:
          success = parse_openBrace(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case closeBrace:
          success = parse_closeBrace(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case entryStatements:
          success = parse_entryStatements(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case entryStatement:
          success = parse_entryStatement(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case location:
          success = parse_location(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case label:
          success = parse_label(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalMetadata:
          success = parse_optionalMetadata(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case pragma:
          success = parse_pragma(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case callprototype:
          success = parse_callprototype(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case calltargets:
          success = parse_calltargets(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case returnTypeList:
          success = parse_returnTypeList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case identifier:
          success = parse_identifier(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case argumentTypeList:
          success = parse_argumentTypeList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case identifierList:
          success = parse_identifierList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case instruction:
          success = parse_instruction(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case registerPrefix:
          success = parse_registerPrefix(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case registerIdentifierList:
          success = parse_registerIdentifierList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case uninitializable:
          success = parse_uninitializable(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case uninitializableDeclaration:
          success = parse_uninitializableDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case arrayDimensions:
          success = parse_arrayDimensions(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case completeEntryStatement:
          success = parse_completeEntryStatement(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case guard:
          success = parse_guard(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case entryName:
          success = parse_entryName(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalArgumentList:
          success = parse_optionalArgumentList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case performanceDirectives:
          success = parse_performanceDirectives(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case ftzInstruction2:
          success = parse_ftzInstruction2(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case ftzInstruction3:
          success = parse_ftzInstruction3(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case approxInstruction2:
          success = parse_approxInstruction2(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case basicInstruction3:
          success = parse_basicInstruction3(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case bfe:
          success = parse_bfe(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case bfi:
          success = parse_bfi(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case bfind:
          success = parse_bfind(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case brev:
          success = parse_brev(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case branch:
          success = parse_branch(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case addOrSub:
          success = parse_addOrSub(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case addCOrSubC:
          success = parse_addCOrSubC(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case atom:
          success = parse_atom(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case bar:
          success = parse_bar(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case barrier:
          success = parse_barrier(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case brkpt:
          success = parse_brkpt(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case clz:
          success = parse_clz(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }

          break;

        case cvt:
          success = parse_cvt(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case cvta:
          success = parse_cvta(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case isspacep:
          success = parse_isspacep(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case div:
          success = parse_div(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case exit:
          success = parse_exit(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case ld:
          success = parse_ld(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case ldu:
          success = parse_ldu(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case mad:
          success = parse_mad(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case mad24:
          success = parse_mad24(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case madc:
          success = parse_madc(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case membar:
          success = parse_membar(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case mov:
          success = parse_mov(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case mul24:
          success = parse_mul24(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case mul:
          success = parse_mul(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case notInstruction:
          success = parse_notInstruction(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case pmevent:
          success = parse_pmevent(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case popc:
          success = parse_popc(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case prefetch:
          success = parse_prefetch(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case prefetchu:
          success = parse_prefetchu(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case prmt:
          success = parse_prmt(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case rcpSqrtInstructio:
          success = parse_rcpSqrtInstructio(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case red:
          success = parse_red(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case ret:
          success = parse_ret(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case sad:
          success = parse_sad(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case selp:
          success = parse_selp(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case set:
          success = parse_set(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case setp:
          success = parse_setp(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case slct:
          success = parse_slct(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case st:
          success = parse_st(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case suld:
          success = parse_suld(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case suq:
          success = parse_suq(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case sured:
          success = parse_sured(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case sust:
          success = parse_sust(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case testp:
          success = parse_testp(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case tex:
          success = parse_tex(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case tld4:
          success = parse_tld4(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case trap:
          success = parse_trap(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case txq:
          success = parse_txq(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case vote:
          success = parse_vote(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case shfl:
          success = parse_shfl(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case addModifier:
          success = parse_addModifier(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case addOrSubOpcode:
          success = parse_addOrSubOpcode(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case dataType:
          success = parse_dataType(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case operand:
          success = parse_operand(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalFloatingRoundNumber:
          success = parse_optionalFloatingRoundNumber(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalFtz:
          success = parse_optionalFtz(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalSaturate:
          success = parse_optionalSaturate(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case constantOperand:
          success = parse_constantOperand(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case nonLabelOperand:
          success = parse_nonLabelOperand(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalVectorIndex:
          success = parse_optionalVectorIndex(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case opcode:
          success = parse_opcode(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case floatRoundingToken:
          success = parse_floatRoundingToken(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case targetElementList:
          success = parse_targetElementList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case targetElement:
          success = parse_targetElement(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case uninitializableAddress:
          success = parse_uninitializableAddress(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case initializable:
          success = parse_initializable(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case arrayDimensionSet:
          success = parse_arrayDimensionSet(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case functionBegin:
          success = parse_functionBegin(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case functionBodyDefinition:
          success = parse_functionBodyDefinition(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalReturnArgument:
          success = parse_optionalReturnArgument(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalReturnArgumentList:
          success = parse_optionalReturnArgumentList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case functionName:
          success = parse_functionName(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case argumentList:
          success = parse_argumentList(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case optionalSemicolon:
          success = parse_optionalSemicolon(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case argumentListBegin:
          success = parse_argumentListBegin(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case argumentListBody:
          success = parse_argumentListBody(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case argumentListEnd:
          success = parse_argumentListEnd(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case argumentDeclaration:
          success = parse_argumentDeclaration(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case parameter:
          success = parse_parameter(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case alignment:
          success = parse_alignment(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case statementVectorType:
          success = parse_statementVectorType(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case initializer:
          success = parse_initializer(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case singleInitializer:
          success = parse_singleInitializer(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case decimalInitializer:
          success = parse_decimalInitializer(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case floatInitializer:
          success = parse_floatInitializer(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case assignment:
          success = parse_assignment(tokens, child_node);
          if (success) {
            node.add(child_node);
          } else {
            current_token_index = current_token_index_placeholder;
            node.reset();
            child_node.reset();
            return false;
          }
          break;

        case TOKEN_COMMA:
          if (!handle_token(TOKEN_COMMA, tokens, child_node, node)) {
            current_token_index = current_token_index_placeholder;
            return false;
          }
          break;

        default:
          std::cout << "Unimplemented token recieved" << std::endl;
          current_token_index = current_token_index_placeholder;
          node.reset();
          child_node.reset();
          return false;
        }
  }
  return true;
}

void Parser::advance(std::vector<Token> tokens) {
  std::cout << "ON TOKEN " << std::to_string(current_token_index) << ": "
            << tokens[current_token_index].text << std::endl;
  current_token_index = current_token_index + 1;
}

bool Parser::parse_statement(std::vector<Token> tokens, Node &node) {
  std::cout << "Attempting To Parse Statement" << std::endl;
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(statement);
  Node child_node;

  success = parse_initializableDeclaration(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_nonEntryStatement(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_entry(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_functionBody(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_functionDeclaration(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }
  return false;
}

bool Parser::parse_initializableDeclaration(std::vector<Token> tokens,
                                            Node &node) {
  bool success;
  node.set_type(initializableDeclaration);
  std::vector<Node_Type> grammar_sequence{
      initializable, addressableVariablePrefix,
      identifier,    arrayDimensions,
      initializer,   TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence, tokens, node);
  return success;
}

bool Parser::parse_nonEntryStatement(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(nonEntryStatement);
  Node child_node;

  success = parse_version(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  return false;
}

bool Parser::parse_initializer(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(initializer);

  std::vector<Node_Type> grammar_sequence_0{assignment, decimalInitializer};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_1{assignment, floatInitializer};
  success = parse_grammar_sequence(grammar_sequence_1, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_2{assignment, singleInitializer};
  success = parse_grammar_sequence(grammar_sequence_2, tokens, node);
  if (success) {
    return true;
  }

  return true;
}

bool Parser::parse_assignment(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(assignment);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_EQUALS) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_entry(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(entry);
  std::vector<Node_Type> grammar_sequence_0{entryDeclaration, openBrace,
                                            entryStatements, closeBrace};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_1{entryDeclaration, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_1, tokens, node);
  if (success) {
    return true;
  }

  return false;
}

bool Parser::parse_functionBegin(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(functionBegin);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_FUNCTION) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_functionBody(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(functionBody);
  Node child_node;

  success = parse_functionBodyDefinition(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();

    if (tokens[current_token_index].token_type == TOKEN_OPENBRACE) {
      child_node.set_type(leaf);
      child_node.set_token(tokens[current_token_index]);
      node.add(child_node);
      child_node.reset();
      advance(tokens);

      success = parse_entryStatements(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();

        if (tokens[current_token_index].token_type == TOKEN_CLOSEBRACE) {
          child_node.set_type(leaf);
          child_node.set_token(tokens[current_token_index]);
          node.add(child_node);
          advance(tokens);
          return true;
        } else {
          node.reset();
          child_node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        node.reset();
        child_node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      node.reset();
      child_node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_functionBodyDefinition(std::vector<Token> tokens,
                                          Node &node) {
  bool success = false;
  node.set_type(functionBodyDefinition);
  std::vector<Node_Type> grammar_sequence_0{externOrVisible, functionBegin,
                                            optionalReturnArgumentList,
                                            functionName, argumentList};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  return success;
}

bool Parser::parse_argumentList(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(argumentList);
  std::vector<Node_Type> grammar_sequence_0{argumentListBegin, argumentListBody,
                                            argumentListEnd};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  return success;
}

bool Parser::parse_argumentListBody(std::vector<Token> tokens, Node &node) {
  bool success = false;
  node.set_type(argumentListBody);
  Node child_node;
  bool end_of_list = false;

  while (!end_of_list) {
    int current_token_index_placeholder0 = current_token_index;
    success = parse_argumentDeclaration(tokens, child_node);
    if (success) {
      int current_token_index_placeholder1 = current_token_index;
      node.add(child_node);
      child_node.reset();
      if (tokens[current_token_index].token_type == TOKEN_COMMA) {
        child_node.set_type(leaf);
        child_node.set_token(tokens[current_token_index]);
        node.add(child_node);
        advance(tokens);
      } else {
        end_of_list = true;
        current_token_index = current_token_index_placeholder1;
      }
    } else {
      end_of_list = true;
      current_token_index = current_token_index_placeholder0;
    }
  }

  return true;
}

bool Parser::parse_argumentDeclaration(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(argumentDeclaration);
  Node child_node;
  int current_token_index_placeholder = current_token_index;

  success = parse_parameter(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();

    success = parse_addressableVariablePrefix(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();

      success = parse_identifier(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();

        success = parse_arrayDimensions(tokens, child_node);
        if (success) {
          node.add(child_node);
          return true;
        } else {
          node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_identifier(std::vector<Token> tokens, Node &node) {
  bool success = false;
  Node child_node;
  node.set_type(identifier);
  int current_token_index_placeholder = current_token_index;

  if (tokens[current_token_index].token_type == TOKEN_UNDERSCORE ||
      tokens[current_token_index].token_type == TOKEN_IDENTIFIER) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  success = parse_opcode(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_parameter(std::vector<Token> tokens, Node &node) {
  bool success = false;
  Node child_node;
  node.set_type(parameter);

  if (tokens[current_token_index].token_type == TOKEN_REG ||
      tokens[current_token_index].token_type == TOKEN_PARAM) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_functionName(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(functionName);
  Node child_node;

  success = parse_identifier(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_functionDeclaration(std::vector<Token> tokens, Node &node) {
  bool success = false;
  node.set_type(functionDeclaration);

  std::vector<Node_Type> grammar_sequence_0{
      externOrVisible, functionBegin, optionalReturnArgumentList,
      functionName,    argumentList,  optionalSemicolon};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_optionalSemicolon(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  Node child_node;
  node.set_type(optionalSemicolon);

  if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
  }

  return true;
}

bool Parser::parse_version(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(version);

  if (tokens[current_token_index].token_type == TOKEN_VERSION) {
    Node version_node;
    version_node.set_type(leaf);
    version_node.set_token(tokens[current_token_index]);
    node.add(version_node);
    advance(tokens);
    if (tokens[current_token_index].token_type == TOKEN_DOUBLE_CONSTANT) {
      Node double_constant_node;
      double_constant_node.set_type(leaf);
      double_constant_node.set_token(tokens[current_token_index]);
      node.add(double_constant_node);
      advance(tokens);
      return true;
    }
  }

  return false;
}

bool Parser::parse_entryDeclaration(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(entryDeclaration);
  Node child_node;

  success = parse_entryName(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();
    success = parse_optionalArgumentList(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      success = parse_performanceDirectives(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();
        return true;
      } else {
        child_node.reset();
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    child_node.reset();
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_openBrace(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(openBrace);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_OPENBRACE) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_closeBrace(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(closeBrace);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_CLOSEBRACE) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_completeEntryStatement(std::vector<Token> tokens,
                                          Node &node) {
  bool success;

  std::vector<std::vector<Node_Type>> grammar_sequences{
      {uninitializableDeclaration},
      {entryStatement},
      {guard, instruction, optionalMetadata},
      {openBrace, entryStatements, closeBrace}};
  for (std::vector<Node_Type> grammar_sequence : grammar_sequences) {
    node.set_type(completeEntryStatement);
    success = parse_grammar_sequence(grammar_sequence, tokens, node);
    if (success) {
      return true;
    }
  }

  return false;
}

bool Parser::parse_uninitializableDeclaration(std::vector<Token> tokens,
                                              Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(uninitializableDeclaration);
  Node child_node;

  success = parse_uninitializable(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();
    success = parse_addressableVariablePrefix(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      success = parse_identifier(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();
        success = parse_arrayDimensions(tokens, child_node);
        if (success) {
          node.add(child_node);
          child_node.reset();
          if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
            child_node.set_type(leaf);
            child_node.set_token(tokens[current_token_index]);
            node.add(child_node);
            child_node.reset();
            advance(tokens);
            return true;
          } else {
            child_node.reset();
            node.reset();
            current_token_index = current_token_index_placeholder;
          }
        } else {
          child_node.reset();
          node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        child_node.reset();
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    child_node.reset();
    node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_entryStatement(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;

  node.set_type(entryStatement);
  Node child_node;

  success = parse_registerDeclaration(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_location(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_label(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_pragma(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_callprototype(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_calltargets(tokens, child_node);
  if (success) {
    node.add(child_node);
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_registerDeclaration(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(registerDeclaration);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_REG) {
    Node reg_node;
    reg_node.set_type(leaf);
    reg_node.set_token(tokens[current_token_index]);
    node.add(reg_node);
    advance(tokens);
    success = parse_registerPrefix(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      success = parse_registerIdentifierList(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();
        if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
          Node semicolon_node;
          semicolon_node.set_type(leaf);
          semicolon_node.set_token(tokens[current_token_index]);
          node.add(semicolon_node);
          advance(tokens);
          return true;
        } else {
          node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        child_node.reset();
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_location(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(location);

  if (tokens[current_token_index].token_type == TOKEN_LOC) {
    Node loc_node;
    loc_node.set_type(leaf);
    loc_node.set_token(tokens[current_token_index]);
    node.add(loc_node);
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
      Node decimal_constant_node;
      decimal_constant_node.set_type(leaf);
      decimal_constant_node.set_token(tokens[current_token_index]);
      node.add(decimal_constant_node);
      advance(tokens);

      if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
        decimal_constant_node.reset();
        decimal_constant_node.set_type(leaf);
        decimal_constant_node.set_token(tokens[current_token_index]);
        node.add(decimal_constant_node);
        advance(tokens);

        if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
          decimal_constant_node.reset();
          decimal_constant_node.set_type(leaf);
          decimal_constant_node.set_token(tokens[current_token_index]);
          node.add(decimal_constant_node);
          advance(tokens);
          return true;
        } else {
          node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_label(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(location);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_LABEL) {
    Node label_node;
    label_node.set_type(leaf);
    label_node.set_token(tokens[current_token_index]);
    node.add(label_node);
    advance(tokens);
    success = parse_optionalMetadata(tokens, child_node);
    if (success) {
      node.add(child_node);
      return true;
    } else {
      node.reset();
      current_token_index = current_token_index_placeholder;
    }

  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_pragma(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(pragma);

  if (tokens[current_token_index].token_type == TOKEN_PRAGMA) {
    Node pragma_node;
    pragma_node.set_type(leaf);
    pragma_node.set_token(tokens[current_token_index]);
    node.add(pragma_node);
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_STRING) {
      Node string_node;
      string_node.set_type(leaf);
      string_node.set_token(tokens[current_token_index]);
      node.add(string_node);
      advance(tokens);

      if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
        Node semicolon_node;
        semicolon_node.set_type(leaf);
        semicolon_node.set_token(tokens[current_token_index]);
        node.add(string_node);
        advance(tokens);
      }

      return true;
    } else {
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_callprototype(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(callprototype);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_LABEL) {
    Node label_node;
    label_node.set_type(leaf);
    label_node.set_token(tokens[current_token_index]);
    node.add(label_node);
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_CALL_PROTOTYPE) {
      Node callprototype_node;
      callprototype_node.set_type(leaf);
      callprototype_node.set_token(tokens[current_token_index]);
      node.add(callprototype_node);
      advance(tokens);
      success = parse_returnTypeList(tokens, child_node);

      if (success) {
        node.add(child_node);
        child_node.reset();
        success = parse_identifier(tokens, child_node);

        if (success) {
          node.add(child_node);
          child_node.reset();
          success = parse_argumentTypeList(tokens, child_node);

          if (success) {
            node.add(child_node);
            child_node.reset();
            if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
              child_node.set_type(leaf);
              child_node.set_token(tokens[current_token_index]);
              node.add(child_node);
              child_node.reset();
              advance(tokens);
              return true;
            } else {
              child_node.reset();
              node.reset();
              current_token_index = current_token_index_placeholder;
            }
          } else {
            child_node.reset();
            node.reset();
            current_token_index = current_token_index_placeholder;
          }

        } else {
          child_node.reset();
          node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        child_node.reset();
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    child_node.reset();
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_calltargets(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(calltargets);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_LABEL) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_CALL_TARGETS) {
      child_node.set_type(leaf);
      child_node.set_token(tokens[current_token_index]);
      node.add(child_node);
      child_node.reset();
      advance(tokens);

      success = parse_identifierList(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();

        if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
          child_node.set_type(leaf);
          child_node.set_token(tokens[current_token_index]);
          node.add(child_node);
          child_node.reset();
          advance(tokens);
        } else {
          child_node.reset();
          node.reset();
          current_token_index = current_token_index_placeholder;
        }
      } else {
        child_node.reset();
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    child_node.reset();
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_addOrSub(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(addOrSub);

  std::vector<Node_Type> grammar_sequence_0{
      addOrSubOpcode, addModifier, dataType, operand,        TOKEN_COMMA,
      operand,        TOKEN_COMMA, operand,  TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }
  return false;
}

bool Parser::parse_addOrSubOpcode(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(addOrSubOpcode);
  Node child_node;

  if (tokens[current_token_index].token_type == OPCODE_ADD) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  if (tokens[current_token_index].token_type == OPCODE_SUB) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_addModifier(std::vector<Token> tokens, Node &node) {
  bool success = false;
  int current_token_index_placeholder = current_token_index;
  node.set_type(addModifier);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_CARRY) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  success = parse_optionalFloatingRoundNumber(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();

    success = parse_optionalFtz(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();

      success = parse_optionalSaturate(tokens, child_node);
      if (success) {
        node.add(child_node);
        child_node.reset();
        return true;
      } else {
        child_node.reset();
        node.reset();
        current_token_index = current_token_index_placeholder;
      }
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    child_node.reset();
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return false;
}

bool Parser::parse_dataType(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(dataType);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_U8 ||
      tokens[current_token_index].token_type == TOKEN_U16 ||
      tokens[current_token_index].token_type == TOKEN_U32 ||
      tokens[current_token_index].token_type == TOKEN_U64 ||
      tokens[current_token_index].token_type == TOKEN_S8 ||
      tokens[current_token_index].token_type == TOKEN_S16 ||
      tokens[current_token_index].token_type == TOKEN_S32 ||
      tokens[current_token_index].token_type == TOKEN_S64 ||
      tokens[current_token_index].token_type == TOKEN_B8 ||
      tokens[current_token_index].token_type == TOKEN_B16 ||
      tokens[current_token_index].token_type == TOKEN_B32 ||
      tokens[current_token_index].token_type == TOKEN_B64 ||
      tokens[current_token_index].token_type == TOKEN_F16 ||
      tokens[current_token_index].token_type == TOKEN_F32 ||
      tokens[current_token_index].token_type == TOKEN_F64) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }
  return false;
}

bool Parser::parse_operand(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(operand);
  Node child_node;
  bool success = false;

  success = parse_constantOperand(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_nonLabelOperand(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();
    return true;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_constantOperand(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(operand);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT ||
      tokens[current_token_index].token_type ==
          TOKEN_UNSIGNED_DECIMAL_CONSTANT ||
      tokens[current_token_index].token_type == TOKEN_DOUBLE_CONSTANT ||
      tokens[current_token_index].token_type == TOKEN_SINGLE_CONSTANT) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_nonLabelOperand(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(operand);
  Node child_node;
  bool success = false;

  success = parse_identifier(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();

    success = parse_optionalVectorIndex(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      return true;
    } else {
      node.reset();
      child_node.reset();
      current_token_index = current_token_index_placeholder;
    }

  } else {
    node.reset();
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  if (tokens[current_token_index].token_type = TOKEN_EXCLAMATION_MARK) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);

    success = parse_identifier(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      return true;
    } else {
      node.reset();
      child_node.reset();
      current_token_index = current_token_index_placeholder;
    }

  } else {
    node.reset();
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_optionalFloatingRoundNumber(std::vector<Token> tokens,
                                               Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalFloatingRoundNumber);
  Node child_node;
  bool success = false;

  // NOT YET IMPLEMENTED

  return true;
}

bool Parser::parse_optionalFtz(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalFtz);
  Node child_node;
  bool success = false;

  // NOT YET IMPLEMENTED

  return true;
}

bool Parser::parse_optionalSaturate(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalSaturate);
  Node child_node;
  bool success = false;

  // NOT YET IMPLEMENTED

  return true;
}

bool Parser::parse_opcode(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(opcode);
  Node child_node;

  if (tokens[current_token_index].token_type == OPCODE_COS ||
      tokens[current_token_index].token_type == OPCODE_SQRT ||
      tokens[current_token_index].token_type == OPCODE_ADD ||
      tokens[current_token_index].token_type == OPCODE_RSQRT ||
      tokens[current_token_index].token_type == OPCODE_ADDC ||
      tokens[current_token_index].token_type == OPCODE_MUL ||
      tokens[current_token_index].token_type == OPCODE_SAD ||
      tokens[current_token_index].token_type == OPCODE_SUB ||
      tokens[current_token_index].token_type == OPCODE_EX2 ||
      tokens[current_token_index].token_type == OPCODE_LG2 ||
      tokens[current_token_index].token_type == OPCODE_RCP ||
      tokens[current_token_index].token_type == OPCODE_SIN ||
      tokens[current_token_index].token_type == OPCODE_REM ||
      tokens[current_token_index].token_type == OPCODE_MUL24 ||
      tokens[current_token_index].token_type == OPCODE_MAD24 ||
      tokens[current_token_index].token_type == OPCODE_DIV ||
      tokens[current_token_index].token_type == OPCODE_ABS ||
      tokens[current_token_index].token_type == OPCODE_NEG ||
      tokens[current_token_index].token_type == OPCODE_MIN ||
      tokens[current_token_index].token_type == OPCODE_MAX ||
      tokens[current_token_index].token_type == OPCODE_MAD ||
      tokens[current_token_index].token_type == OPCODE_MADC ||
      tokens[current_token_index].token_type == OPCODE_SET ||
      tokens[current_token_index].token_type == OPCODE_SETP ||
      tokens[current_token_index].token_type == OPCODE_SELP ||
      tokens[current_token_index].token_type == OPCODE_SLCT ||
      tokens[current_token_index].token_type == OPCODE_MOV ||
      tokens[current_token_index].token_type == OPCODE_ST ||
      tokens[current_token_index].token_type == OPCODE_COPYSIGN ||
      tokens[current_token_index].token_type == OPCODE_SHFL ||
      tokens[current_token_index].token_type == OPCODE_CVT ||
      tokens[current_token_index].token_type == OPCODE_CVTA ||
      tokens[current_token_index].token_type == OPCODE_ISSPACEP ||
      tokens[current_token_index].token_type == OPCODE_AND ||
      tokens[current_token_index].token_type == OPCODE_XOR ||
      tokens[current_token_index].token_type == OPCODE_OR ||
      tokens[current_token_index].token_type == OPCODE_BRA ||
      tokens[current_token_index].token_type == OPCODE_CALL ||
      tokens[current_token_index].token_type == OPCODE_RET ||
      tokens[current_token_index].token_type == OPCODE_EXIT ||
      tokens[current_token_index].token_type == OPCODE_TRAP ||
      tokens[current_token_index].token_type == OPCODE_BRKPT ||
      tokens[current_token_index].token_type == OPCODE_SUBC ||
      tokens[current_token_index].token_type == OPCODE_TEX ||
      tokens[current_token_index].token_type == OPCODE_LD ||
      tokens[current_token_index].token_type == OPCODE_LDU ||
      tokens[current_token_index].token_type == OPCODE_SULD ||
      tokens[current_token_index].token_type == OPCODE_TXQ ||
      tokens[current_token_index].token_type == OPCODE_SUST ||
      tokens[current_token_index].token_type == OPCODE_SURED ||
      tokens[current_token_index].token_type == OPCODE_SUQ ||
      tokens[current_token_index].token_type == OPCODE_ATOM ||
      tokens[current_token_index].token_type == OPCODE_RED ||
      tokens[current_token_index].token_type == OPCODE_NOT ||
      tokens[current_token_index].token_type == OPCODE_CNOT ||
      tokens[current_token_index].token_type == OPCODE_VOTE ||
      tokens[current_token_index].token_type == OPCODE_SHR ||
      tokens[current_token_index].token_type == OPCODE_SHL ||
      tokens[current_token_index].token_type == OPCODE_MEMBAR ||
      tokens[current_token_index].token_type == OPCODE_FMA ||
      tokens[current_token_index].token_type == OPCODE_PMEVENT ||
      tokens[current_token_index].token_type == OPCODE_POPC ||
      tokens[current_token_index].token_type == OPCODE_CLZ ||
      tokens[current_token_index].token_type == OPCODE_BFIND ||
      tokens[current_token_index].token_type == OPCODE_BREV ||
      tokens[current_token_index].token_type == OPCODE_BFI ||
      tokens[current_token_index].token_type == OPCODE_TESTP ||
      tokens[current_token_index].token_type == OPCODE_TLD4 ||
      tokens[current_token_index].token_type == OPCODE_PREFETCH ||
      tokens[current_token_index].token_type == OPCODE_PREFETCHU) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_floatRoundingToken(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(floatingRoundingToken);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_RN ||
      tokens[current_token_index].token_type == TOKEN_RM ||
      tokens[current_token_index].token_type == TOKEN_RP ||
      tokens[current_token_index].token_type == TOKEN_RZ) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_entryName(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(entryName);
  Node child_node;

  // Not Yet Implemented
  return false;
}

bool Parser::parse_optionalArgumentList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalArgumentList);
  Node child_node;

  // Not Yet Implemented

  return true;
}

bool Parser::parse_performanceDirectives(std::vector<Token> tokens,
                                         Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(performanceDirectives);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_optionalMetadata(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalMetadata);
  Node child_node;

  // Not Yet Implemented

  return true;
}

bool Parser::parse_guard(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(guard);
  Node child_node;

  // Not Yet Implemented

  return true;
}

bool Parser::parse_uninitializable(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(uninitializable);
  Node child_node;
  bool success = false;

  success = parse_externOrVisible(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();

    success = parse_uninitializableAddress(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      return true;
    } else {
      node.reset();
      child_node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return false;
}

bool Parser::parse_initializable(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(initializable);
  bool success = false;
  std::vector<Node_Type> grammar_sequence{externOrVisible,
                                          initializableAddress};
  success = parse_grammar_sequence(grammar_sequence, tokens, node);
  return success;
}

bool Parser::parse_arrayDimensions(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(arrayDimensions);
  Node child_node;
  bool success = false;

  success = parse_arrayDimensionSet(tokens, child_node);
  if (success) {
    node.add(child_node);
    child_node.reset();
  } else {
    current_token_index = current_token_index_placeholder;
  }

  return true;
}

bool Parser::parse_arrayOperand(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(arrayOperand);

  std::vector<Node_Type> grammar_sequence_0{operand};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_1{TOKEN_OPENBRACE, identifierList,
                                            TOKEN_CLOSEBRACE};
  success = parse_grammar_sequence(grammar_sequence_1, tokens, node);
  if (success) {
    return true;
  }
}

bool Parser::parse_addressableVariablePrefix(std::vector<Token> tokens,
                                             Node &node) {
  node.set_type(addressableVariablePrefix);
  bool success;

  std::vector<Node_Type> grammar_sequence_0{alignment, dataType,
                                            statementVectorType};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_1{alignment, statementVectorType,
                                            dataType};
  success = parse_grammar_sequence(grammar_sequence_1, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_2{dataType, statementVectorType,
                                            alignment};
  success = parse_grammar_sequence(grammar_sequence_2, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_3{dataType, alignment,
                                            statementVectorType};
  success = parse_grammar_sequence(grammar_sequence_3, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_4{statementVectorType, dataType,
                                            alignment};
  success = parse_grammar_sequence(grammar_sequence_4, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_5{statementVectorType, alignment,
                                            dataType};
  success = parse_grammar_sequence(grammar_sequence_5, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_6{dataType, parameterAttribute,
                                            alignment};
  success = parse_grammar_sequence(grammar_sequence_6, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_7{dataType, statementVectorType};
  success = parse_grammar_sequence(grammar_sequence_7, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_8{statementVectorType, dataType};
  success = parse_grammar_sequence(grammar_sequence_8, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_9{dataType, alignment};
  success = parse_grammar_sequence(grammar_sequence_9, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_10{alignment, dataType};
  success = parse_grammar_sequence(grammar_sequence_10, tokens, node);
  if (success) {
    return true;
  }

  std::vector<Node_Type> grammar_sequence_11{dataType};
  success = parse_grammar_sequence(grammar_sequence_11, tokens, node);
  if (success) {
    return true;
  }

  return false;
}

bool Parser::parse_statementVectorType(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(statementVectorType);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_V2 ||
      tokens[current_token_index].token_type == TOKEN_V4) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }
  return false;
}

bool Parser::parse_alignment(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(alignment);
  std::vector<Node_Type> grammar_sequence_0{TOKEN_ALIGN,
                                            TOKEN_DECIMAL_CONSTANT};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }
}

bool Parser::parse_uninitializableAddress(std::vector<Token> tokens,
                                          Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(uninitializableAddress);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_LOCAL ||
      tokens[current_token_index].token_type == TOKEN_SHARED ||
      tokens[current_token_index].token_type == TOKEN_PARAM) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_initializableAddress(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(initializableAddress);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_CONST ||
      tokens[current_token_index].token_type == TOKEN_GLOBAL) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_returnTypeList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(returnTypeList);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_optionalReturnArgument(std::vector<Token> tokens,
                                          Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_argumentListBegin(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(argumentListBegin);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_OPENPARENTHESIS) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_argumentListEnd(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(argumentListEnd);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_CLOSEPARENTHESIS) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_registerPrefix(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(registerPrefix);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_identifierList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(identifierList);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_arrayDimensionSet(std::vector<Token> tokens, Node &node) {
  bool success = true;
  bool finished_1_iteration = false;
  node.set_type(arrayDimensionSet);
  Node temp_node;

  while (success) {
    std::vector<Node_Type> grammar_sequence_0{
        TOKEN_OPENBRACKET, TOKEN_DECIMAL_CONSTANT, TOKEN_CLOSEBRACKET};
    success = parse_grammar_sequence(grammar_sequence_0, tokens, temp_node);

    if (!success) {
      std::vector<Node_Type> grammar_sequence_1{TOKEN_OPENBRACKET,
                                                TOKEN_CLOSEBRACKET};
      success = parse_grammar_sequence(grammar_sequence_1, tokens, temp_node);
    }

    if (success) {
      finished_1_iteration = true;
      for (Node node_ : temp_node.get_children()) {
        node.add(node_);
        temp_node.reset();
      }
    }
  }

  return finished_1_iteration;
}

bool Parser::parse_entryStatements(std::vector<Token> tokens, Node &node) {
  bool success = true;
  bool finished_1_iteration = false;
  node.set_type(entryStatements);
  Node temp_node;

  while (success) {
    std::vector<Node_Type> grammar_sequence_0{completeEntryStatement};
    success = parse_grammar_sequence(grammar_sequence_0, tokens, temp_node);

    if (success) {
      finished_1_iteration = true;
      for (Node node_ : temp_node.get_children()) {
        node.add(node_);
        temp_node.reset();
      }
    }
  }

  return finished_1_iteration;
}

bool Parser::parse_argumentTypeList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(argumentTypeList);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_registerIdentifierList(std::vector<Token> tokens,
                                          Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(registerIdentifierList);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_optionalVectorIndex(std::vector<Token> tokens, Node &node) {
  node.set_type(optionalVectorIndex);
  bool success;

  // std::vector<Node_Type> grammar_sequence_0 { vectorIndex };
  // success = parse_grammar_sequence(grammar_sequence_0, tokens, node);

  return true;
}

bool Parser::parse_targetElementList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(targetElementList);
  Node child_node;
  bool success = false;

  bool end_of_list = false;
  while (!end_of_list) {
    int current_token_index_checkpoint0 = current_token_index;
    success = parse_targetElement(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();

      if (tokens[current_token_index].token_type == TOKEN_COMMA) {
        child_node.set_type(leaf);
        child_node.set_token(tokens[current_token_index]);
        node.add(child_node);
        child_node.reset();
        advance(tokens);
      } else {
        end_of_list = true;
        child_node.reset();
      }
    } else {
      end_of_list = true;
      child_node.reset();
      current_token_index = current_token_index_checkpoint0;
    }
  }
  return true;
}

bool Parser::parse_targetElement(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(targetElement);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_target(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(target);
  Node child_node;
  bool success = false;

  if (tokens[current_token_index].token_type == TOKEN_TARGET) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();

    success = parse_targetElementList(tokens, child_node);
    if (success) {
      node.add(child_node);
      child_node.reset();
      return true;
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  }

  return false;
}

bool Parser::parse_externOrVisible(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(externOrVisible);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_WEAK ||
      tokens[current_token_index].token_type == TOKEN_EXTERN ||
      tokens[current_token_index].token_type == TOKEN_VISIBLE) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
  }
  return true;
}

bool Parser::parse_preprocessor(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(preprocessor);
  Node child_node;

  if (tokens[current_token_index].token_type == PREPROCESSOR_INCLUDE ||
      tokens[current_token_index].token_type == PREPROCESSOR_DEFINE ||
      tokens[current_token_index].token_type == PREPROCESSOR_IF ||
      tokens[current_token_index].token_type == PREPROCESSOR_IFDEF ||
      tokens[current_token_index].token_type == PREPROCESSOR_ELSE ||
      tokens[current_token_index].token_type == PREPROCESSOR_ENDIF ||
      tokens[current_token_index].token_type == PREPROCESSOR_LINE ||
      tokens[current_token_index].token_type == PREPROCESSOR_FILE) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return true;
  }

  return false;
}

bool Parser::parse_samplerDeclaration(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(samplerDeclaration);

  std::vector<Node_Type> grammar_sequence_0{externOrVisible, textureSpace,
                                            TOKEN_SAMPLEREF, identifier,
                                            TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_surfaceDeclaration(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(surfaceDeclaration);

  std::vector<Node_Type> grammar_sequence_0{externOrVisible, textureSpace,
                                            TOKEN_SURFREF, identifier,
                                            TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_textureDeclaration(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(textureDeclaration);

  std::vector<Node_Type> grammar_sequence_0{
      externOrVisible, textureSpace, TOKEN_TEXREF, identifier, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_globalSharedDeclaration(std::vector<Token> tokens,
                                           Node &node) {
  bool success;
  node.set_type(globalSharedDeclaration);

  std::vector<Node_Type> grammar_sequence_0{
      externOrVisible, TOKEN_SHARED,    addressableVariablePrefix,
      identifier,      arrayDimensions, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_globalLocalDeclaration(std::vector<Token> tokens,
                                          Node &node) {
  bool success;
  node.set_type(globalLocalDeclaration);

  std::vector<Node_Type> grammar_sequence_0{
      externOrVisible, TOKEN_LOCAL,     addressableVariablePrefix,
      identifier,      arrayDimensions, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_addressSize(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(addressSize);

  std::vector<Node_Type> grammar_sequence_0{TOKEN_ADDRESS_SIZE,
                                            TOKEN_DECIMAL_CONSTANT};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_ftzInstruction2(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_ftzInstruction3(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_approxInstruction2(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_basicInstruction3(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_bfe(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_bfi(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_bfind(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_brev(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_branch(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(branch);

  std::vector<Node_Type> grammar_sequence_0{OPCODE_BRA, optionalUni,
                                            branchOperand, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };

  std::vector<Node_Type> grammar_sequence_1{call};
  success = parse_grammar_sequence(grammar_sequence_1, tokens, node);
  if (success) {
    return true;
  };

  return false;
}

bool Parser::parse_addCOrSubC(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(addCOrSubC);

  std::vector<Node_Type> grammar_sequence_0{
      addOrSubOpcode, addCModifier, dataType, operand,        TOKEN_COMMA,
      operand,        TOKEN_COMMA,  operand,  TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_atom(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(atom);

  std::vector<Node_Type> grammar_sequence_0{
      OPCODE_ATOM,       atomModifier,  atomicOpcode,
      dataType,          operand,       TOKEN_COMMA,
      TOKEN_OPENBRACKET, memoryOperand, TOKEN_CLOSEBRACKET,
      TOKEN_COMMA,       operand,       TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };

  std::vector<Node_Type> grammar_sequence_1{
      OPCODE_ATOM,       atomModifier,   atomicOpcode,
      dataType,          operand,        TOKEN_COMMA,
      TOKEN_OPENBRACKET, memoryOperand,  TOKEN_CLOSEBRACKET,
      TOKEN_COMMA,       operand,        TOKEN_COMMA,
      operand,           TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_1, tokens, node);
  if (success) {
    return true;
  };

  return false;
}

bool Parser::parse_bar(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(bar);

  std::vector<Node_Type> grammar_sequence_0{OPCODE_BAR, barrierOperation,
                                            optionalBarrierOperation,
                                            operandSequence, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };
  return false;
}

bool Parser::parse_barrier(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_brkpt(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_clz(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_cvt(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_cvta(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_isspacep(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_div(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_exit(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_ld(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_ldu(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_mad(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_mad24(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_madc(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_membar(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_mov(std::vector<Token> tokens, Node &node) {
  node.set_type(mov);
  bool success;

  std::vector<Node_Type> grammar_sequence_0{OPCODE_MOV,       dataType,
                                            arrayOperand,     TOKEN_COMMA,
                                            movSourceOperand, TOKEN_SEMICOLON};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  }

  return false;
}

bool Parser::parse_mul24(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_mul(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_notInstruction(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_pmevent(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_popc(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_prefetch(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_prefetchu(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_prmt(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_rcpSqrtInstructio(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_red(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_ret(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_vectorIndex(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_sad(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_selp(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_set(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_setp(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_slct(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_st(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_suld(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_suq(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_sured(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_sust(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_testp(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_tex(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_tld4(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_trap(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_fileDeclaration(std::vector<Token> tokens, Node &node) {
  bool success;
  node.set_type(fileDeclaration);

  std::vector<Node_Type> grammar_sequence_0{TOKEN_FILE, TOKEN_DECIMAL_CONSTANT,
                                            TOKEN_STRING,
                                            optionalTimestampAndSize};
  success = parse_grammar_sequence(grammar_sequence_0, tokens, node);
  if (success) {
    return true;
  };

  return false;
}

bool Parser::parse_txq(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_vote(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_shfl(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalReturnArgument);
  Node child_node;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_decimalInitializer(std::vector<Token> tokens, Node &node) {
  node.set_type(decimalInitializer);
  bool success;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_floatInitializer(std::vector<Token> tokens, Node &node) {
  node.set_type(decimalInitializer);
  bool success;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_optionalReturnArgumentList(std::vector<Token> tokens,
                                              Node &node) {
  node.set_type(optionalReturnArgumentList);
  bool success;

  // Not Yet Implemented

  return true;
}

bool Parser::parse_singleInitializer(std::vector<Token> tokens, Node &node) {
  node.set_type(decimalInitializer);
  bool success;

  // Not Yet Implemented

  return false;
}

bool Parser::parse_instruction(std::vector<Token> tokens, Node &node) {
  node.set_type(instruction);
  bool success;

  std::vector<std::vector<Node_Type>> grammar_sequences{{ftzInstruction2},
                                                        {ftzInstruction3},
                                                        {approxInstruction2},
                                                        {basicInstruction3},
                                                        {bfe},
                                                        {bfi},
                                                        {bfind},
                                                        {brev},
                                                        {branch},
                                                        {addOrSub},
                                                        {addCOrSubC},
                                                        {atom},
                                                        {bar},
                                                        {brkpt},
                                                        {clz},
                                                        {cvt},
                                                        {cvta},
                                                        {isspacep},
                                                        {div},
                                                        {exit},
                                                        {ld},
                                                        {ldu},
                                                        {mad},
                                                        {mad24},
                                                        {madc},
                                                        {membar},
                                                        {mov},
                                                        {mul24},
                                                        {mul},
                                                        {notInstruction},
                                                        {pmevent},
                                                        {popc},
                                                        {prefetch},
                                                        {prefetchu},
                                                        {prmt},
                                                        {rcpSqrtInstruction},
                                                        {red},
                                                        {ret},
                                                        {sad},
                                                        {selp},
                                                        {set},
                                                        {setp},
                                                        {slct},
                                                        {st},
                                                        {suld},
                                                        {suq},
                                                        {sured},
                                                        {sust},
                                                        {testp},
                                                        {tex},
                                                        {tld4},
                                                        {trap},
                                                        {txq},
                                                        {vote},
                                                        {shfl}};
  for (std::vector<Node_Type> grammar_sequence : grammar_sequences) {
    success = parse_grammar_sequence(grammar_sequence, tokens, node);
    if (success) {
      return true;
    }
  }

  return false;
}

} // namespace ptx_parser