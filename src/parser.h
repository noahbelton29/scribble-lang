#pragma once

#include "ast.h"
#include "lexer.h"

#include <cstddef>
#include <memory>
#include <vector>

class Parser {
public:
  explicit Parser(std::vector<Token> tokens);
  std::vector<std::unique_ptr<ASTNode>> parse();

private:
  std::vector<Token> tokens;
  std::size_t position{0};

  Token current() const;
  Token consume();
  Token expect(TokenType type);

  std::unique_ptr<VarDecl> parseVarDecl();
  std::unique_ptr<ASTNode> parseStatement();
};