# Notes on how parser is implemented

the ptx-parser will parse a ptx file.

first a lexer will divide the text into tokens using regex

next the parser will parse it into a parse tree.  the parse tree data structure is a series of nodes that has a `Node_Type` that denotes what kind of gramatical structure it represets.  If the node is of Type `leaf`, it will have a token, converted from raw text in the file.  else, it will have a list of child nodes.

the root node is always type 'statement'

# how to write a grammar rule

1. ensure the grammer rule has a corresponding node type in `Lexer.hpp`

2. declare function in `Parser.hpp`.  function shount be in the format `bool parse_<grammerRuleName>(std::vector<Token> tokens, Node &node);`

3. add a switch case in `Parser::parse_grammar_sequence`.  how the case handles can be copied from the other cases depending on if it is a leaf or not

3.1 

grammar case

```
case statement:
        success = parse_statement(tokens, child_node); // attempt to parse the next token(s) as a statement.  if successful, child node will be turned into a statement node, and will be recursivley filled in
        if(success) {
          node.add(child_node); // add the child node to the nodes children if it was a statement
        } else {
          current_token_index = current_token_index_placeholder; // reset everything and return false, so the calling function can move on to the next possibility 
          node.reset();
          child_node.reset();
          return false;
        }
        break;
```

3.2 token case

```
case TOKEN_CLOSEBRACKET:
        if (!handle_token(TOKEN_CLOSEBRACKET, tokens, child_node, node)) { //pass to handle_node, which will create a leaf node out of the token if the token is a TOKEN_CLOSEBRACE in this case
          current_token_index = current_token_index_placeholder; // if not, return false
          return false;
        }
        break;

```

4. implament function

example

```
bool Parser::parse_completeEntryStatement(std::vector<Token> tokens,
                                          Node &node) {
  bool success;
  node.set_type(completeEntryStatement);  // set node type

  std::vector<std::vector<Node_Type>> grammar_sequences {{uninitializableDeclaration}, {entryStatement}, {guard, instruction, optionalMetadata}, {openBrace, entryStatements, closeBrace}};  define list of possible forms this grammer can take.  if there is only one possibility, you dont need a vector of vectors
  for(std::vector<Node_Type> grammar_sequence : grammar_sequences) {
    success = parse_grammar_sequence(grammar_sequence, tokens, node); // if successful, there will be a child node for each grammar in the node, else, the node and current token will be reset
    if(success) {return true;}
  }

  return false;
}
```

BUG: if the fist attempt at parsing is unsuccessful, the node type will be wiped.  
2 possible fixes Im considering.  either don't clear nodetype on reset.  when a new grammar is parsed, the node type is automatically set, so should theoretically work.  Other solution is to set the node type before every call to parse_grammar_sequence.

dealing with lists

lists can have an arbitrary number of items

example

```
bool Parser::parse_arrayDimensionSet(std::vector<Token> tokens, Node &node) {
  bool success = true;
  bool finished_1_iteration = false;
  node.set_type(arrayDimensionSet);
  Node temp_node;

  while(success) {
    std::vector<Node_Type> grammar_sequence_0{
      TOKEN_OPENBRACKET, TOKEN_DECIMAL_CONSTANT, TOKEN_CLOSEBRACKET};
    success = parse_grammar_sequence(grammar_sequence_0, tokens, temp_node);

    if(!success) {
      std::vector<Node_Type> grammar_sequence_1 {TOKEN_OPENBRACKET, TOKEN_CLOSEBRACKET};
      success = parse_grammar_sequence(grammar_sequence_1, tokens, temp_node);
    }

    if(success) {
      finished_1_iteration = true;
      for (Node node_ : temp_node.get_children()) {
        node.add(node_);
        temp_node.reset();
      }
    }
  }

  return finished_1_iteration;
}
```

finished 1 iteration proves that you do have a list of at least length one
a temp node is used, so that the entire list doesnt get wiped when you reach the end and fail to parse another item