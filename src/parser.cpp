#include "parser.h"
#include "ast.h"
#include "lexer.h"

#include <memory>
#include <stdexcept>
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
  Returns the current token and advances the position
*/
Token Parser::consume() { return tokens[position++]; }

/*
  Consumes a token of the expected type or throws an error
*/
Token Parser::expect(TokenType type) {
  if (current().type != type) {
    throw std::runtime_error("unexpected token '" + current().value +
                             "' on line " + std::to_string(current().line));
  }
  return consume();
}

/*
  Parses a variable declaration of the form: var <name> = <number>;
*/
std::unique_ptr<VarDecl> Parser::parseVarDecl() {
  expect(TokenType::Var);
  Token name = expect(TokenType::Identifier);

  expect(TokenType::Equals);
  Token number = expect(
      TokenType::Number); // TODO: add string, boolean, and float literal types

  auto literal = std::make_unique<NumberLiteral>();
  try {
    literal->value = std::stoull(number.value);
  } catch (const std::out_of_range &) {
    throw std::runtime_error("number literal '" + number.value +
                             "' out of range on line " +
                             std::to_string(number.line));
  } // TODO: split this into a bounds checker

  expect(TokenType::Semicolon);
  auto varDecl = std::make_unique<VarDecl>();
  varDecl->name = name.value;
  varDecl->value = std::move(literal);
  return varDecl;
}

/*
  Parses a single statement based on the current token type
*/
std::unique_ptr<ASTNode> Parser::parseStatement() {
  switch (current().type) {
  case TokenType::Var:
    return parseVarDecl();
  default:
    throw std::runtime_error("unexpected token '" + current().value +
                             "' on line " + std::to_string(current().line));
  }
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