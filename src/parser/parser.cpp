#include "parser.h"
#include "ast.h"
#include "error.h"
#include "lexer.h"
#include "token_printer.h"

#include <memory>
#include <string>
#include <vector>

/*
  Constructs a Parser object with a vector of tokens to parse
*/
Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

/*
  Returns the current token without consuming it
*/
Token Parser::current() const { return tokens[position]; }

/*
  Returns the token ahead of the current token without consuming it
*/
Token Parser::peek() const { return tokens[position + 1]; }

/*
  Returns the current token and advances the position
*/
Token Parser::consume() { return tokens[position++]; }

/*
  Consumes a token of the expected type or throws an error
*/
Token Parser::expect(TokenType type) {
  if (current().type != type) {
    throw ScribbleError("parser",
                        "expected '" + tokenTypeName(type) + "' but got '" +
                            current().value + "'",
                        current().line, current().column);
  }
  return consume();
}

/*
  Parses all statements until end of file and returns them as a list of nodes
*/
std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
  std::vector<std::unique_ptr<ASTNode>> nodes;
  while (current().type != TokenType::EndOfFile) {
    nodes.push_back(parseStatement());
  }
  return nodes;
}