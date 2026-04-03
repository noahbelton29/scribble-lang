#pragma once

#include "ast.h"

#include <cstdint>
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
  uint64_t interpret();

private:
  const std::vector<std::unique_ptr<ASTNode>> &nodes;
  std::unordered_map<std::string, uint64_t> variables{};

  uint64_t evaluate(const ASTNode *node);
};