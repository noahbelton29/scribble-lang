#pragma once

#include "ast.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
  std::unordered_map<std::string, Value> variables{};
  std::unordered_set<std::string> constants{};

  Value evaluate(const ASTNode *node);
};