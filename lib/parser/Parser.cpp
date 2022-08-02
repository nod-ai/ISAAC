#include "Parser.hpp"

namespace ptx_parser {

std::string Node::repr() {
  std::string output = "";
  if (children.size() > 0) {
    output = output + "(";
  }
  if (value.token_type == TOKEN_UNRECOGNIZED) {
    output = output + value.text;
  }
  if (children.size() > 0) {
    for (Node child : children) {
      output = output + child.repr();
    }
    output = output + ")";
  }
}

void Parser::parse(std::vector<Token> tokens) {
  current_token_index = 0;
  int success = 0;
  Node child_node;

  success = parse_statement(tokens, child_node);

  if (success == 1) {
    parse_tree.add(child_node);
    std::cout << "SUCCESS" << std::endl;
  } else {
    std::cout << "ERROR" << std::endl;
  }
}

void Parser::advance(std::vector<Token> tokens) {
  current_token_index = current_token_index + 1;
  std::cout << "Current Index = " + std::to_string(current_token_index)
            << std::endl;
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
      child_node.set_type(semicolon_leaf);
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
    version_node.set_type(version_leaf);
    version_node.set_token(tokens[current_token_index]);
    node.add(version_node);
    advance(tokens);
    if (tokens[current_token_index].token_type == TOKEN_DOUBLE_CONSTANT) {
      Node double_constant_node;
      double_constant_node.set_type(double_constant_leaf);
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
    child_node.set_type(openBrace_leaf);
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
    child_node.set_type(closeBrace_leaf);
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
            child_node.set_type(semicolon_leaf);
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
    reg_node.set_type(reg_leaf);
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
          semicolon_node.set_type(semicolon_leaf);
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
    loc_node.set_type(loc_leaf);
    loc_node.set_token(tokens[current_token_index]);
    node.add(loc_node);
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
      Node decimal_constant_node;
      decimal_constant_node.set_type(decimalConstant_leaf);
      decimal_constant_node.set_token(tokens[current_token_index]);
      node.add(decimal_constant_node);
      advance(tokens);

      if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
        decimal_constant_node.reset();
        decimal_constant_node.set_type(decimalConstant_leaf);
        decimal_constant_node.set_token(tokens[current_token_index]);
        node.add(decimal_constant_node);
        advance(tokens);

        if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
          decimal_constant_node.reset();
          decimal_constant_node.set_type(decimalConstant_leaf);
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
    label_node.set_type(label_leaf);
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
    pragma_node.set_type(pragma_leaf);
    pragma_node.set_token(tokens[current_token_index]);
    node.add(pragma_node);
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_STRING) {
      Node string_node;
      string_node.set_type(string_leaf);
      string_node.set_token(tokens[current_token_index]);
      node.add(string_node);
      advance(tokens);

      if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
        Node semicolon_node;
        semicolon_node.set_type(semicolon_leaf);
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
    label_node.set_type(label_leaf);
    label_node.set_token(tokens[current_token_index]);
    node.add(label_node);
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_CALL_PROTOTYPE) {
      Node callprototype_node;
      callprototype_node.set_type(callprototype_leaf);
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
              child_node.set_type(semicolon_leaf);
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
    child_node.set_type(label_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);

    if (tokens[current_token_index].token_type == TOKEN_CALL_TARGETS) {
      child_node.set_type(calltargets_leaf);
      child_node.set_token(tokens[current_token_index]);
      node.add(child_node);
      child_node.reset();
      advance(tokens);

      success = parse_identifierList(tokens, child_node);
      if (success == 1) {
        node.add(child_node);
        child_node.reset();

        if (tokens[current_token_index].token_type == TOKEN_SEMICOLON) {
          child_node.set_type(semicolon_leaf);
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
            child_node.set_type(comma_leaf);
            child_node.set_token(tokens[current_token_index]);
            node.add(child_node);
            child_node.reset();
            advance(tokens);

            success = parse_operand(tokens, child_node);
            if (success == 1) {
              node.add(child_node);
              child_node.reset();

              if (tokens[current_token_index].token_type == TOKEN_COMMA) {
                child_node.set_type(comma_leaf);
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
                    child_node.set_type(semicolon_leaf);
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
    child_node.set_type(opcodeAdd_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == OPCODE_SUB) {
    child_node.set_type(opcodeSub_leaf);
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
    child_node.set_type(carry_leaf);
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

  if (tokens[current_token_index].token_type == TOKEN_U8) {
    child_node.set_type(u8_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_U16) {
    child_node.set_type(u16_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_U32) {
    child_node.set_type(u32_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_U64) {
    child_node.set_type(u64_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_S8) {
    child_node.set_type(s8_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_S16) {
    child_node.set_type(s16_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_S32) {
    child_node.set_type(s32_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_S64) {
    child_node.set_type(s64_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_B8) {
    child_node.set_type(b8_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_B16) {
    child_node.set_type(b16_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_B32) {
    child_node.set_type(b32_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_B64) {
    child_node.set_type(b64_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_F16) {
    child_node.set_type(f16_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_F32) {
    child_node.set_type(f32_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_F64) {
    child_node.set_type(f64_leaf);
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

  if (tokens[current_token_index].token_type == TOKEN_DECIMAL_CONSTANT) {
    child_node.set_type(decimalConstant_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type ==
      TOKEN_UNSIGNED_DECIMAL_CONSTANT) {
    child_node.set_type(unsignedDecimalConstant_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_DOUBLE_CONSTANT) {
    child_node.set_type(doubleConstant_leaf);
    child_node.set_token(tokens[current_token_index]);
    node.add(child_node);
    child_node.reset();
    advance(tokens);
    return 1;
  }

  if (tokens[current_token_index].token_type == TOKEN_SINGLE_CONSTANT) {
    child_node.set_type(singleConstant_leaf);
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
    child_node.set_type(exclamationMark_leaf);
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

} // namespace ptx_parser
