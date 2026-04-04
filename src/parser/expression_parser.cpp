#include "error.h"
#include "parser.h"

/*
  Parses a number literal and checks it fits within uint64 bounds
*/
std::unique_ptr<NumberLiteral> Parser::parseNumberLiteral(const Token &num) {
  auto literal = std::make_unique<NumberLiteral>();
  try {
    literal->value = std::stoll(num.value);
  } catch (const std::out_of_range &) {
    throw ScribbleError("parser",
                        "number literal '" + num.value + "' out of range",
                        num.line, num.column);
  }
  literal->line = num.line;
  literal->column = num.column;
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
    throw ScribbleError("parser",
                        "float literal '" + flt.value + "' out of range",
                        flt.line, flt.column);
  }
  literal->line = flt.line;
  literal->column = flt.column;
  return literal;
}

/*
  Parses a primary expression: literals, identifiers, function calls, or a
  parenthesised sub-expression
*/
std::unique_ptr<ASTNode> Parser::parsePrimary() {
  // Parenthesised group: ( expr )
  if (current().type == TokenType::LParen) {
    consume();
    auto expr = parseExpression();
    expect(TokenType::RParen);
    return expr;
  }
  if (current().type == TokenType::Minus) {
    Token minus = consume();
    auto operand = parsePrimary();
    auto zero = std::make_unique<NumberLiteral>();
    zero->value = 0;
    zero->line = minus.line;
    zero->column = minus.column;
    auto expr = std::make_unique<BinaryExpr>();
    expr->op = "-";
    expr->line = minus.line;
    expr->column = minus.column;
    expr->left = std::move(zero);
    expr->right = std::move(operand);
    return expr;
  }
  if (current().type == TokenType::NumberLiteral) {
    Token num = consume();
    return parseNumberLiteral(num);
  }
  if (current().type == TokenType::FloatLiteral) {
    Token flt = consume();
    return parseFloatLiteral(flt);
  }
  if (current().type == TokenType::StringLiteral) {
    Token str = consume();
    auto lit = std::make_unique<StringLiteral>();
    lit->value = str.value;
    lit->line = str.line;
    lit->column = str.column;
    return lit;
  }
  if (current().type == TokenType::True) {
    Token tok = consume();
    auto lit = std::make_unique<BoolLiteral>();
    lit->value = true;
    lit->line = tok.line;
    lit->column = tok.column;
    return lit;
  }
  if (current().type == TokenType::False) {
    Token tok = consume();
    auto lit = std::make_unique<BoolLiteral>();
    lit->value = false;
    lit->line = tok.line;
    lit->column = tok.column;
    return lit;
  }
  if (current().type == TokenType::Identifier) {
    if (peek().type == TokenType::LParen)
      return parseFuncCall();
    Token ident = consume();
    auto identifier = std::make_unique<Identifier>();
    identifier->name = ident.value;
    identifier->line = ident.line;
    identifier->column = ident.column;
    return identifier;
  }
  throw ScribbleError("parser", "invalid expression '" + current().value + "'",
                      current().line, current().column);
}

/*
  Parses * and /
*/
std::unique_ptr<ASTNode> Parser::parseTerm() {
  auto left = parsePrimary();
  while (current().type == TokenType::Star ||
         current().type == TokenType::Slash) {
    Token op = consume();
    auto right = parsePrimary();
    auto expr = std::make_unique<BinaryExpr>();
    expr->op = op.value;
    expr->line = op.line;
    expr->column = op.column;
    expr->left = std::move(left);
    expr->right = std::move(right);
    left = std::move(expr);
  }
  return left;
}

/*
  Parses + and -
*/
std::unique_ptr<ASTNode> Parser::parseAddSub() {
  auto left = parseTerm();
  while (current().type == TokenType::Plus ||
         current().type == TokenType::Minus) {
    Token op = consume();
    auto right = parseTerm();
    auto expr = std::make_unique<BinaryExpr>();
    expr->op = op.value;
    expr->line = op.line;
    expr->column = op.column;
    expr->left = std::move(left);
    expr->right = std::move(right);
    left = std::move(expr);
  }
  return left;
}

/*
  Parses comparison operators
*/
std::unique_ptr<ASTNode> Parser::parseExpression() {
  auto left = parseAddSub();
  if (current().type == TokenType::EqualsEquals ||
      current().type == TokenType::NotEquals ||
      current().type == TokenType::LessThan ||
      current().type == TokenType::GreaterThan ||
      current().type == TokenType::LessThanEquals ||
      current().type == TokenType::GreaterThanEquals) {
    Token op = consume();
    auto right = parseAddSub();
    auto expr = std::make_unique<BinaryExpr>();
    expr->op = op.value;
    expr->line = op.line;
    expr->column = op.column;
    expr->left = std::move(left);
    expr->right = std::move(right);
    return expr;
  }
  return left;
}