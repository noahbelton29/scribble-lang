#include "interpreter.h"
#include "ast.h"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

/*
  Constructs an Interpreter with a reference to the parsed AST nodes
*/
Interpreter::Interpreter(const std::vector<std::unique_ptr<ASTNode>> &nodes)
    : nodes(nodes) {}

/*
  Recursively evaluates an AST node and returns its value
*/
uint64_t Interpreter::evaluate(const ASTNode *node) {
  if (auto *lit = dynamic_cast<const NumberLiteral *>(node)) {
    return lit->value;
  }
  if (auto *decl = dynamic_cast<const VarDecl *>(node)) {
    uint64_t value = evaluate(decl->value.get());
    variables[decl->name] = value;
    return value;
  }
  if (auto *ident = dynamic_cast<const Identifier *>(node)) {
    return variables[ident->name];
  }

  /*
    Evaluates both sides of a binary expression and applies the operator
  */
  if (auto *binaryExpr = dynamic_cast<const BinaryExpr *>(node)) {
    uint64_t left = evaluate(binaryExpr->left.get());
    uint64_t right = evaluate(binaryExpr->right.get());
    if (binaryExpr->op == "+")
      return left + right;
    if (binaryExpr->op == "-")
      return left - right;
    if (binaryExpr->op == "*")
      return left * right;
    if (binaryExpr->op == "/") {
      if (right == 0)
        throw std::runtime_error("division by zero caught");
      return left / right;
    }
  }
  throw std::runtime_error("unknown AST node");
}

/*
  Evaluates all AST nodes and returns the result of the last one
*/
uint64_t Interpreter::interpret() {
  uint64_t result = 0;
  for (const auto &node : nodes) {
    result = evaluate(node.get());
  }
  return result;
}