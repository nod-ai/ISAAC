#include "Parser.hpp"

namespace ptx_parser {

std::string Node::repr() {
  std::string output = "";
  if (children.size() > 0) {
    output = output + "(";
  }
  if (node_type == leaf) {
    std::cout << "found leaf" << std::endl;
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
  int success = 0;
  Node child_node;

  while (current_token_index < tokens.size()) {

    success = parse_statement(tokens, child_node);

    if (success == 1) {
      parse_tree.add(child_node);
      std::cout << "SUCCESS" << std::endl;
    } else {
      std::cout << "ERROR" << std::endl;
    }
  }
}

void Parser::advance(std::vector<Token> tokens) {
  current_token_index = current_token_index + 1;
}

int Parser::parse_statement(std::vector<Token> tokens, Node &node) {
  std::cout << "Attempting To Parse Statement" << std::endl;
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(statement);
  Node child_node;

  success = parse_initializableDeclaration(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_nonEntryStatement(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_entry(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_functionBody(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  success = parse_functionDeclaration(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }
  return 0;
}

int Parser::parse_initializableDeclaration(std::vector<Token> tokens,
                                           Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(initializableDeclaration);
  Node child_node;

  // NOT YET IMPLEMENTED

  return 0;
}

int Parser::parse_nonEntryStatement(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(nonEntryStatement);
  Node child_node;

  success = parse_version(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
  }

  return 0;
}

int Parser::parse_entry(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(entry);
  Node child_node;

  success = parse_entryDeclaration(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    success = parse_openBrace(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      int token_index_1 = current_token_index;
      success = parse_entryStatements(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();
      } else {
        child_node.reset();
        current_token_index = token_index_1;
      }
      success = parse_closeBrace(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        return 1;
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

  success = parse_entryDeclaration(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
      child_node.set_type(leaf);
      child_node.set_token(tokens[current_token_index]);
      node.add(child_node);
      child_node.reset();
      advance(tokens);
      return 1;
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

  return 0;
}

int Parser::parse_functionBody(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(functionBody);
  Node child_node;

  // NOT YET IMPLEMENTED

  return 0;
}

int Parser::parse_functionDeclaration(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(functionDeclaration);
  Node child_node;

  // NOT YET IMPLEMENTED

  return 0;
}

int Parser::parse_version(std::vector<Token> tokens, Node &node) {
  int success = 0;
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
      return 1;
    }
  }

  return 0;
}

int Parser::parse_entryDeclaration(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(entryDeclaration);
  Node child_node;

  success = parse_entryName(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    success = parse_optionalArgumentList(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      success = parse_performanceDirectives(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();
        return 1;
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
  return 0;
}

int Parser::parse_openBrace(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(openBrace);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_OPENBRACE) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return 1;
  }

  return 0;
}

int Parser::parse_closeBrace(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(closeBrace);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_CLOSEBRACE) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    advance(tokens);
    return 1;
  }

  return 0;
}

int Parser::parse_entryStatements(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(entryStatements);
  Node child_node;

  success = parse_completeEntryStatement(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
  } else {
    child_node.reset();
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  if (success == 1) {
    int token_index_checkpoint_0 = current_token_index;
    success = parse_entryStatements(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      return 1;
    } else {
      child_node.reset();
      node.reset();
      current_token_index = token_index_checkpoint_0;
    }
    return 1;
  }
  return 0;
}

int Parser::parse_completeEntryStatement(std::vector<Token> tokens,
                                         Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(completeEntryStatement);
  Node child_node;

  success = parse_uninitializableDeclaration(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_entryStatement(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_guard(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    success = parse_instruction(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      success = parse_optionalMetadata(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        return 1;
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

  success = parse_openBrace(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    success = parse_entryStatements(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      success = parse_closeBrace(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        return 1;
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
  return 0;
}

int Parser::parse_uninitializableDeclaration(std::vector<Token> tokens,
                                             Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(uninitializableDeclaration);
  Node child_node;

  success = parse_uninitializable(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    success = parse_addressableVariablePrefix(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      success = parse_identifier(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();
        success = parse_arrayDimensions(tokens, child_node);
        if (success == 1) {
          node.add(child_node);
          child_node.reset();
          if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
            child_node.set_type(leaf);
            child_node.set_token(tokens[current_token_index]);
            node.add(child_node);
            child_node.reset();
            advance(tokens);
            return 1;
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

  return 0;
}

int Parser::parse_entryStatement(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;

  node.set_type(entryStatement);
  Node child_node;

  success = parse_registerDeclaration(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_location(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_label(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_pragma(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_callprototype(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_calltargets(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return 0;
}

int Parser::parse_registerDeclaration(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;
  node.set_type(registerDeclaration);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKRN_REG) {
    Node reg_node;
    reg_node.set_type(leaf);
    reg_node.set_token(tokens[current_token_index]);
    node.add(reg_node);
    advance(tokens);
    success = parse_registerPrefix(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      success = parse_registerIdentifierList(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();
        if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
          Node semicolon_node;
          semicolon_node.set_type(leaf);
          semicolon_node.set_token(tokens[current_token_index]);
          node.add(semicolon_node);
          advance(tokens);
          return 1;
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
  return 0;
}

int Parser::parse_location(std::vector<Token> tokens, Node &node) {
  int success = 0;
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
          return 1;
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
  return 0;
}

int Parser::parse_label(std::vector<Token> tokens, Node &node) {
  int success = 0;
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
    if (success == 1) {
      node.add(child_node);
      return 1;
    } else {
      node.reset();
      current_token_index = current_token_index_placeholder;
    }

  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return 0;
}

int Parser::parse_pragma(std::vector<Token> tokens, Node &node) {
  int success = 0;
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

      return 1;
    } else {
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  } else {
    node.reset();
    current_token_index = current_token_index_placeholder;
  }
  return 0;
}

int Parser::parse_callprototype(std::vector<Token> tokens, Node &node) {
  int success = 0;
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

      if (success == 1) {
        node.add(child_node);
        child_node.reset();
        success = parse_identifier(tokens, child_node);

        if (success == 1) {
          node.add(child_node);
          child_node.reset();
          success = parse_argumentTypeList(tokens, child_node);

          if (success == 1) {
            node.add(child_node);
            child_node.reset();
            if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
              child_node.set_type(leaf);
              child_node.set_token(tokens[current_token_index]);
              node.add(child_node);
              child_node.reset();
              advance(tokens);
              return 1;
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
  return 0;
}

int Parser::parse_calltargets(std::vector<Token> tokens, Node &node) {
  int success = 0;
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
      if (success == 1) {
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
  return 0;
}

int Parser::parse_instruction(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;
  node.set_type(instruction);
  Node child_node;

  success = parse_addOrSub(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  // success = parse_ftzInstruction2(tokens, child_node);
  // if(success == 1) {
  // 	node.add(child_node);
  // 	return 1;
  // } else {
  // 	child_node.reset();
  // 	current_token_index = current_token_index_placeholder;
  // }

  // success = parse_ftzInstruction3(tokens, child_node);
  // if(success == 1) {
  // 	node.add(child_node);
  // 	return 1;
  // } else {
  // 	child_node.reset();
  // 	current_token_index = current_token_index_placeholder;
  // }

  return 0;
}

int Parser::parse_addOrSub(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;
  node.set_type(addOrSub);
  Node child_node;

  success = parse_addOrSubOpcode(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();

    success = parse_addModifier(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();

      success = parse_dataType(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();

        success = parse_operand(tokens, child_node);
        if (success == 1) {
          node.add(child_node);
          child_node.reset();

          if (tokens[current_token_index].token_type == TOKEN_COMMA) {
            child_node.set_type(leaf);
            child_node.set_token(tokens[current_token_index]);
            node.add(child_node);
            child_node.reset();
            advance(tokens);

            success = parse_operand(tokens, child_node);
            if (success == 1) {
              node.add(child_node);
              child_node.reset();

              if (tokens[current_token_index].token_type == TOKEN_COMMA) {
                child_node.set_type(leaf);
                child_node.set_token(tokens[current_token_index]);
                node.add(child_node);
                child_node.reset();
                advance(tokens);

                success = parse_operand(tokens, child_node);
                if (success == 1) {
                  node.add(child_node);
                  child_node.reset();

                  if (tokens[current_token_index].token_type ==
                      TOKEN_SEMICOLON) {
                    child_node.set_type(leaf);
                    child_node.set_token(tokens[current_token_index]);
                    node.add(child_node);
                    child_node.reset();
                    advance(tokens);
                    return 1;
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
              current_token_index - current_token_index_placeholder;
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
  return 0;
}

int Parser::parse_addOrSubOpcode(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;
  node.set_type(addOrSubOpcode);
  Node child_node;

  if (tokens[current_token_index].token_type == OPCODE_ADD) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == OPCODE_SUB) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  return 0;
}

int Parser::parse_addModifier(std::vector<Token> tokens, Node &node) {
  int success = 0;
  int current_token_index_placeholder = current_token_index;
  node.set_type(addModifier);
  Node child_node;

  if (tokens[current_token_index].token_type == TOKEN_CARRY) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  success = parse_optionalFloatingRoundNumber(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();

    success = parse_optionalFtz(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();

      success = parse_optionalSaturate(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();
        return 1;
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
  return 0;
}

int Parser::parse_dataType(std::vector<Token> tokens, Node &node) {
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
    return 1;
  }
  return 0;
}

int Parser::parse_operand(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(operand);
  Node child_node;
  int success = 0;

  success = parse_constantOperand(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  success = parse_nonLabelOperand(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
    return 1;
  } else {
    child_node.reset();
    current_token_index = current_token_index_placeholder;
  }

  return 0;
}

int Parser::parse_constantOperand(std::vector<Token> tokens, Node &node) {
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
    return 1;
  }

  return 0;
}

int Parser::parse_nonLabelOperand(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(operand);
  Node child_node;
  int success = 0;

  success = parse_identifier(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();

    success = parse_optionalVectorIndex(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      return 1;
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
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      return 1;
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

  return 0;
}

int Parser::parse_optionalFloatingRoundNumber(std::vector<Token> tokens,
                                              Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalFloatingRoundNumber);
  Node child_node;
  int success = 0;

  // NOT YET IMPLEMENTED

  return 1;
}

int Parser::parse_optionalFtz(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalFtz);
  Node child_node;
  int success = 0;

  // NOT YET IMPLEMENTED

  return 1;
}

int Parser::parse_optionalSaturate(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalSaturate);
  Node child_node;
  int success = 0;

  // NOT YET IMPLEMENTED

  return 1;
}

int Parser::parse_identifier(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(identifier);
  Node child_node;
  int success = 0;

  if (tokens[current_token_index].token_type == TOKEN_UNDERSCORE ||
      tokens[current_token_index].token_type == TOKEN_IDENTIFIER) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  success = parse_opcode(tokens, child_node);
  if (success == 1) {
    node.add(child_node);
    child_node.reset();
  } else {
    current_token_index = current_token_index_placeholder;
    child_node.reset();
    node.reset();
  }

  return 0;
}

int Parser::parse_opcode(std::vector<Token> tokens, Node &node) {
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
    return 1;
  }

  return 0;
}

int Parser::parse_floatRoundingToken(std::vector<Token> tokens, Node &node) {
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
    return 1;
  }

  return 0;
}

int Parser::parse_entryName(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(entryName);
  Node child_node;

  // Not Yet Implemented
  return 0;
}

int Parser::parse_optionalArgumentList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalArgumentList);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_performanceDirectives(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(performanceDirectives);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_optionalMetadata(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalMetadata);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_guard(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(guard);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_uninitializable(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(uninitializable);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_arrayDimensions(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(arrayDimensions);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_returnTypeList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(returnTypeList);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_addressableVariablePrefix(std::vector<Token> tokens,
                                            Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(addressableVariablePrefix);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_registerPrefix(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(registerPrefix);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_argumentTypeList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(argumentTypeList);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_registerIdentifierList(std::vector<Token> tokens,
                                         Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(registerIdentifierList);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_identifierList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(identifierList);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_optionalVectorIndex(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(optionalVectorIndex);
  Node child_node;

  // Not Yet Implemented

  return 1;
}

int Parser::parse_targetElementList(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(targetElementList);
  Node child_node;
  int success = 0;

  bool end_of_list = false;
  while (!end_of_list) {
    int current_token_index_checkpoint0 = current_token_index;
    success = parse_targetElement(tokens, child_node);
    if (success == 1) {
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
  return 1;
}

int Parser::parse_targetElement(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(targetElement);
  Node child_node;

  // Not Yet Implemented

  return 0;
}

int Parser::parse_target(std::vector<Token> tokens, Node &node) {
  int current_token_index_placeholder = current_token_index;
  node.set_type(target);
  Node child_node;
  int success = 0;

  if (tokens[current_token_index].token_type == TOKEN_TARGET) {
    child_node.set_type(leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();

    success = parse_targetElementList(tokens, child_node);
    if (success == 1) {
      node.add(child_node);
      child_node.reset();
      return 1;
    } else {
      child_node.reset();
      node.reset();
      current_token_index = current_token_index_placeholder;
    }
  }

  return 0;
}

int Parser::parse_externOrVisible(std::vector<Token> tokens, Node &node) {
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
    return 1;
  }
  return 0;
}

} // namespace ptx_parser