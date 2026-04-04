#pragma once

#include "ast.h"
#include "environment.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/*
  Walks the AST and evaluates each node, storing variables and returning results
*/
class Interpreter {
public:
  Interpreter(const std::vector<std::unique_ptr<ASTNode>> &nodes);
  void interpret();

private:
  const std::vector<std::unique_ptr<ASTNode>> &nodes;
  std::shared_ptr<Environment> globalEnv;
  std::shared_ptr<Environment> env;
  std::unordered_map<std::string, const FuncDecl *> functions{};

  Value evaluate(const ASTNode *node);
};