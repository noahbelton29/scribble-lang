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
  Parses a floating point literal and checks it fits within bounds
*/
std::unique_ptr<FloatLiteral> Parser::parseFloatLiteral(const Token &flt) {
  auto literal = std::make_unique<FloatLiteral>();
  try {
    literal->value = std::stod(flt.value);
  } catch (const std::out_of_range &) {
    throw std::runtime_error("float literal '" + flt.value +
                             "' out of range on line " +
                             std::to_string(flt.line));
  }
  return literal;
}

/*
  Parses a print statement in the form of: print() or println()
*/
std::unique_ptr<PrintStmt> Parser::parsePrintStmt() {
  bool newline = current().type == TokenType::Println;
  consume();

  expect(TokenType::LParen);

  auto value = parseExpression();

  expect(TokenType::RParen);
  expect(TokenType::Semicolon);

  auto stmt = std::make_unique<PrintStmt>();
  stmt->value = std::move(value);
  stmt->newline = newline;
  return stmt;
}

/*
  Parses a constant variable declaration of the form: const <name> = <expr>;
*/
std::unique_ptr<ConstDecl> Parser::parseConstDecl() {
  expect(TokenType::Const);

  Token name = expect(TokenType::Identifier);
  expect(TokenType::Equals);

  auto value = parseExpression();
  expect(TokenType::Semicolon);

  auto constDecl = std::make_unique<ConstDecl>();
  constDecl->name = name.value;
  constDecl->value = std::move(value);
  return constDecl;
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
  Parses an expression, which can be a literal, identifier, or binary operation
*/
std::unique_ptr<ASTNode> Parser::parseExpression() {
  std::unique_ptr<ASTNode> left;

  switch (current().type) {
  case TokenType::NumberLiteral: {
    Token num = consume();
    left = parseNumberLiteral(num);
    break;
  }
  case TokenType::FloatLiteral: {
    Token flt = consume();
    left = parseFloatLiteral(flt);
    break;
  }
  case TokenType::StringLiteral: {
    Token str = consume();
    auto strLiteral = std::make_unique<StringLiteral>();
    strLiteral->value = str.value;
    left = std::move(strLiteral);
    break;
  }
  case TokenType::True: {
    consume();
    auto lit = std::make_unique<BoolLiteral>();
    lit->value = true;
    return lit;
  }
  case TokenType::False: {
    consume();
    auto lit = std::make_unique<BoolLiteral>();
    lit->value = false;
    return lit;
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

  // If the next token is a comparison operator, wrap the left side in a
  // BinaryExpr
  if (current().type == TokenType::EqualsEquals ||
      current().type == TokenType::NotEquals ||
      current().type == TokenType::LessThan ||
      current().type == TokenType::GreaterThan ||
      current().type == TokenType::LessThanEquals ||
      current().type == TokenType::GreaterThanEquals) {
    Token op = consume();
    auto right = parseExpression();
    auto expr = std::make_unique<BinaryExpr>();
    expr->left = std::move(left);
    expr->op = op.value;
    expr->right = std::move(right);
    return expr;
  }

  // If the next token is a binary operator, wrap the left side in a
  // BinaryExpr
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
  case TokenType::Identifier: {
    if (peek().type == TokenType::Equals)
      return parseAssignment();
  }
  case TokenType::Var:
    return parseVarDecl();
  case TokenType::Const:
    return parseConstDecl();
  case TokenType::Print:
  case TokenType::Println:
    return parsePrintStmt();
  default:
    throw std::runtime_error("unexpected token '" + current().value +
                             "' on line " + std::to_string(current().line));
  }
}

/*
  Parses reassignment between a variable and a new value of the form: x =
  <expr>;
*/
std::unique_ptr<AssignStmt> Parser::parseAssignment() {
  Token name = expect(TokenType::Identifier);
  expect(TokenType::Equals);

  auto value = parseExpression();
  expect(TokenType::Semicolon);

  auto literal = std::make_unique<AssignStmt>();
  literal->name = name.value;
  literal->value = std::move(value);
  return literal;
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