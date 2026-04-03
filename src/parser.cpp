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
  Parses a number literal and checks it fits within uint64 bounds
*/
std::unique_ptr<NumberLiteral> Parser::parseNumberLiteral(const Token &num) {
  auto literal = std::make_unique<NumberLiteral>();
  try {
    literal->value = std::stoull(num.value);
  } catch (const std::out_of_range &) {
    throw std::runtime_error("number literal '" + num.value +
                             "' out of range on line " +
                             std::to_string(num.line));
  }
  return literal;
}

/*
  Parses a variable declaration of the form: var <name> = <expr>;
*/
std::unique_ptr<VarDecl> Parser::parseVarDecl() {
  expect(TokenType::Var);

  Token name = expect(TokenType::Identifier);
  expect(TokenType::Equals);

  auto value = parseExpression();
  expect(TokenType::Semicolon);

  auto varDecl = std::make_unique<VarDecl>();
  varDecl->name = name.value;
  varDecl->value = std::move(value);
  return varDecl;
}

/*
  Parses an expression, which can be a number, identifier, or binary operation
*/
std::unique_ptr<ASTNode> Parser::parseExpression() {
  std::unique_ptr<ASTNode> left;

  switch (current().type) {
  case TokenType::Number: {
    Token num = consume();
    left = parseNumberLiteral(num);
    break;
  }
  case TokenType::Identifier: {
    Token ident = consume();
    auto identifier = std::make_unique<Identifier>();
    identifier->name = ident.value;
    left = std::move(identifier);
    break;
  }
  default:
    throw std::runtime_error("invalid expression: " + current().value);
  }

  // If the next token is a binary operator, wrap the left side in a BinaryExpr
  if (current().type == TokenType::Plus || current().type == TokenType::Minus ||
      current().type == TokenType::Star || current().type == TokenType::Slash) {
    Token op = consume();
    auto right = parseExpression();
    auto expr = std::make_unique<BinaryExpr>();
    expr->left = std::move(left);
    expr->op = op.value;
    expr->right = std::move(right);
    return expr;
  }

  return left;
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