#include "ast.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

/*
  Parses a function call of the form: name(a, b); or name(a, b)
*/
std::unique_ptr<FuncCall> Parser::parseFuncCall() {
  Token name = expect(TokenType::Identifier);
  expect(TokenType::LParen);

  std::vector<std::unique_ptr<ASTNode>> args;
  while (current().type != TokenType::RParen &&
         current().type != TokenType::EndOfFile) {
    args.push_back(parseExpression());
    if (current().type == TokenType::Comma)
      consume();
  }

  expect(TokenType::RParen);

  auto call = std::make_unique<FuncCall>();
  call->name = name.value;
  call->line = name.line;
  call->column = name.column;
  call->args = std::move(args);
  return call;
}

/*
  Parses a function declaration of the form: func name(a, b) { ... }
*/
std::unique_ptr<FuncDecl> Parser::parseFuncDecl() {
  expect(TokenType::Func);
  Token name = expect(TokenType::Identifier);
  expect(TokenType::LParen);

  std::vector<std::string> params;
  while (current().type != TokenType::RParen &&
         current().type != TokenType::EndOfFile) {
    params.push_back(expect(TokenType::Identifier).value);
    if (current().type == TokenType::Comma)
      consume();
  }

  expect(TokenType::RParen);
  expect(TokenType::LBrace);

  std::vector<std::unique_ptr<ASTNode>> body;
  while (current().type != TokenType::RBrace &&
         current().type != TokenType::EndOfFile) {
    body.push_back(parseStatement());
  }

  expect(TokenType::RBrace);

  auto decl = std::make_unique<FuncDecl>();
  decl->name = name.value;
  decl->line = name.line;
  decl->column = name.column;
  decl->params = std::move(params);
  decl->body = std::move(body);
  return decl;
}

/*
  Parses a while statment of the form: while <expr> { ... }
*/
std::unique_ptr<WhileStmt> Parser::parseWhileStmt() {
  Token tok = expect(TokenType::While);
  auto condition = parseExpression();
  expect(TokenType::LBrace);

  std::vector<std::unique_ptr<ASTNode>> body;
  while (current().type != TokenType::RBrace &&
         current().type != TokenType::EndOfFile) {
    body.push_back(parseStatement());
  }

  expect(TokenType::RBrace);

  auto stmt = std::make_unique<WhileStmt>();
  stmt->condition = std::move(condition);
  stmt->line = tok.line;
  stmt->column = tok.column;
  stmt->body = std::move(body);
  return stmt;
}

/*
  Parses an if statment of the form: if <expr> { ... }
*/
std::unique_ptr<IfStmt> Parser::parseIfStmt() {
  Token tok = expect(TokenType::If);
  auto condition = parseExpression();
  expect(TokenType::LBrace);

  std::vector<std::unique_ptr<ASTNode>> body;
  while (current().type != TokenType::RBrace &&
         current().type != TokenType::EndOfFile) {
    body.push_back(parseStatement());
  }

  expect(TokenType::RBrace);

  std::vector<std::unique_ptr<ASTNode>> elseBody;
  if (current().type == TokenType::Else) {
    consume();
    if (current().type == TokenType::If) {
      elseBody.push_back(parseIfStmt());
    } else {
      expect(TokenType::LBrace);
      while (current().type != TokenType::RBrace &&
             current().type != TokenType::EndOfFile) {
        elseBody.push_back(parseStatement());
      }
      expect(TokenType::RBrace);
    }
  }

  auto stmt = std::make_unique<IfStmt>();
  stmt->condition = std::move(condition);
  stmt->line = tok.line;
  stmt->column = tok.column;
  stmt->body = std::move(body);
  stmt->elseBody = std::move(elseBody);
  return stmt;
}

/*
  Parses a print statement in the form of: print() or println()
*/
std::unique_ptr<PrintStmt> Parser::parsePrintStmt() {
  bool newline = current().type == TokenType::Println;
  Token tok = consume();

  expect(TokenType::LParen);
  auto value = parseExpression();
  expect(TokenType::RParen);
  expect(TokenType::Semicolon);

  auto stmt = std::make_unique<PrintStmt>();
  stmt->value = std::move(value);
  stmt->newline = newline;
  stmt->line = tok.line;
  stmt->column = tok.column;
  return stmt;
}

/*
  Parses a single statement based on the current token type
*/
std::unique_ptr<ASTNode> Parser::parseStatement() {
  switch (current().type) {
  case TokenType::Identifier: {
    if (peek().type == TokenType::LParen) {
      auto call = parseFuncCall();
      expect(TokenType::Semicolon);
      return call;
    }
    if (peek().type == TokenType::Equals)
      return parseAssignment();
    if (peek().type == TokenType::PlusEquals ||
        peek().type == TokenType::MinusEquals ||
        peek().type == TokenType::StarEquals ||
        peek().type == TokenType::SlashEquals)
      return parseCompoundAssignment();
    throw ScribbleError("parser", "unexpected token '" + current().value + "'",
                        current().line, current().column);
  }
  case TokenType::Var:
    return parseVarDecl();
  case TokenType::Const:
    return parseConstDecl();
  case TokenType::If:
    return parseIfStmt();
  case TokenType::While:
    return parseWhileStmt();
  case TokenType::Func:
    return parseFuncDecl();
  case TokenType::Print:
  case TokenType::Println:
    return parsePrintStmt();
  default:
    throw ScribbleError("parser", "unexpected token '" + current().value + "'",
                        current().line, current().column);
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
  literal->line = name.line;
  literal->column = name.column;
  literal->value = std::move(value);
  return literal;
}

/*
  Parses compound assignment of the form e.g. x += 5;
*/
std::unique_ptr<AssignStmt> Parser::parseCompoundAssignment() {
  Token name = expect(TokenType::Identifier);
  Token op = consume();
  auto value = parseExpression();
  expect(TokenType::Semicolon);

  auto ident = std::make_unique<Identifier>();
  ident->name = name.value;
  ident->line = name.line;
  ident->column = name.column;

  auto binaryExpr = std::make_unique<BinaryExpr>();
  binaryExpr->left = std::move(ident);
  binaryExpr->op = std::string(1, op.value[0]);
  binaryExpr->line = op.line;
  binaryExpr->column = op.column;
  binaryExpr->right = std::move(value);

  auto stmt = std::make_unique<AssignStmt>();
  stmt->name = name.value;
  stmt->line = name.line;
  stmt->column = name.column;
  stmt->value = std::move(binaryExpr);
  return stmt;
}