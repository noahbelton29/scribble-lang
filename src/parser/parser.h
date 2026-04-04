#pragma once

#include "ast.h"
#include "lexer.h"

#include <cstddef>
#include <memory>
#include <vector>

/*
  Recursively descends through the list of tokens and converts it to an AST
*/
class Parser {
public:
  explicit Parser(std::vector<Token> tokens);
  std::vector<std::unique_ptr<ASTNode>> parse();

private:
  std::vector<Token> tokens;
  std::size_t position{0};

  Token current() const;
  Token peek() const;
  Token consume();
  Token expect(TokenType type);

  std::unique_ptr<ReturnStmt> parseReturnStmt();
  std::unique_ptr<FuncCall> parseFuncCall();
  std::unique_ptr<FuncDecl> parseFuncDecl();
  std::unique_ptr<WhileStmt> parseWhileStmt();
  std::unique_ptr<IfStmt> parseIfStmt();
  std::unique_ptr<PrintStmt> parsePrintStmt();
  std::unique_ptr<ConstDecl> parseConstDecl();
  std::unique_ptr<VarDecl> parseVarDecl();
  std::unique_ptr<ASTNode> parseStatement();
  std::unique_ptr<ASTNode> parseExpression();
  std::unique_ptr<AssignStmt> parseAssignment();
  std::unique_ptr<AssignStmt> parseCompoundAssignment();
  std::unique_ptr<NumberLiteral> parseNumberLiteral(const Token &num);
  std::unique_ptr<FloatLiteral> parseFloatLiteral(const Token &flt);
};