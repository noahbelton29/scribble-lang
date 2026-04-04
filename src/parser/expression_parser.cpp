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
  Parses an expression, which can be a literal, identifier, or binary operation
*/
std::unique_ptr<ASTNode> Parser::parseExpression() {
  std::unique_ptr<ASTNode> left;
  switch (current().type) {
  case TokenType::Minus: {
    Token minus = consume();
    if (current().type == TokenType::NumberLiteral) {
      Token num = consume();
      auto literal = parseNumberLiteral(num);
      literal->value = -literal->value;
      left = std::move(literal);
    } else if (current().type == TokenType::FloatLiteral) {
      Token flt = consume();
      auto literal = parseFloatLiteral(flt);
      literal->value = -literal->value;
      left = std::move(literal);
    } else {
      throw ScribbleError("parser", "expected number after '-'", minus.line,
                          minus.column);
    }
    break;
  }
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
    strLiteral->line = str.line;
    strLiteral->column = str.column;
    left = std::move(strLiteral);
    break;
  }
  case TokenType::True: {
    Token tok = consume();
    auto lit = std::make_unique<BoolLiteral>();
    lit->value = true;
    lit->line = tok.line;
    lit->column = tok.column;
    return lit;
  }
  case TokenType::False: {
    Token tok = consume();
    auto lit = std::make_unique<BoolLiteral>();
    lit->value = false;
    lit->line = tok.line;
    lit->column = tok.column;
    return lit;
  }
  case TokenType::Identifier: {
    Token ident = consume();
    auto identifier = std::make_unique<Identifier>();
    identifier->name = ident.value;
    identifier->line = ident.line;
    identifier->column = ident.column;
    left = std::move(identifier);
    break;
  }
  default:
    throw ScribbleError("parser",
                        "invalid expression '" + current().value + "'",
                        current().line, current().column);
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
    expr->line = op.line;
    expr->column = op.column;
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
    expr->line = op.line;
    expr->column = op.column;
    expr->right = std::move(right);
    return expr;
  }
  return left;
}