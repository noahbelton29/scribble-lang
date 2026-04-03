#include "interpreter.h"
#include "ast.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

/*
  Constructs an Interpreter with a reference to the parsed AST nodes
*/
Interpreter::Interpreter(const std::vector<std::unique_ptr<ASTNode>> &nodes)
    : nodes(nodes) {}

/*
  Recursively evaluates an AST node and returns its value
*/
Value Interpreter::evaluate(const ASTNode *node) {
  if (auto *lit = dynamic_cast<const NumberLiteral *>(node)) {
    return lit->value;
  }
  if (auto *lit = dynamic_cast<const FloatLiteral *>(node)) {
    return lit->value;
  }
  if (auto *decl = dynamic_cast<const VarDecl *>(node)) {
    Value value = evaluate(decl->value.get());
    variables[decl->name] = value;
    return value;
  }
  if (auto *decl = dynamic_cast<const ConstDecl *>(node)) {
    Value value = evaluate(decl->value.get());
    variables[decl->name] = value;
    constants.insert(decl->name);
    return value;
  }
  if (auto *ident = dynamic_cast<const Identifier *>(node)) {
    if (!variables.count(ident->name))
      throw std::runtime_error("undefined variable '" + ident->name + "'");
    return variables[ident->name];
  }
  if (auto *lit = dynamic_cast<const BoolLiteral *>(node)) {
    return lit->value;
  }
  if (auto *lit = dynamic_cast<const StringLiteral *>(node)) {
    return lit->value;
  }
  if (auto *stmt = dynamic_cast<const PrintStmt *>(node)) {
    Value value = evaluate(stmt->value.get());
    std::visit(
        [&](auto visitor) {
          if constexpr (std::is_same_v<decltype(visitor), bool>)
            std::cout << (visitor ? "true" : "false");
          else
            std::cout << visitor;
        },
        value);
    if (stmt->newline)
      std::cout << std::endl;
    return value;
  }
  if (auto *stmt = dynamic_cast<const AssignStmt *>(node)) {
    if (constants.count(stmt->name))
      throw std::runtime_error("cannot reassign const '" + stmt->name + "'");
    if (!variables.count(stmt->name))
      throw std::runtime_error("undefined variable '" + stmt->name + "'");
    Value value = evaluate(stmt->value.get());
    variables[stmt->name] = value;
    return value;
  }

  /*
    Evaluates both sides of a binary expression and applies the operator
  */
  if (auto *binaryExpr = dynamic_cast<const BinaryExpr *>(node)) {
    Value left = evaluate(binaryExpr->left.get());
    Value right = evaluate(binaryExpr->right.get());

    return std::visit(
        [&](auto lft, auto rht) -> Value {
          if constexpr (!std::is_arithmetic_v<decltype(lft)> ||
                        !std::is_arithmetic_v<decltype(rht)> ||
                        std::is_same_v<decltype(lft), bool> ||
                        std::is_same_v<decltype(rht), bool>)
            throw std::runtime_error("type mismatch: cannot apply '" +
                                     binaryExpr->op +
                                     "' to incompatible types");
          else if (binaryExpr->op == "+")
            return lft + rht;
          else if (binaryExpr->op == "-")
            return lft - rht;
          else if (binaryExpr->op == "*")
            return lft * rht;
          else if (binaryExpr->op == "/")
            return lft / rht;
          else if (binaryExpr->op == "==")
            return lft == rht;
          else if (binaryExpr->op == "!=")
            return lft != rht;
          else if (binaryExpr->op == "<")
            return lft < rht;
          else if (binaryExpr->op == ">")
            return lft > rht;
          else if (binaryExpr->op == "<=")
            return lft <= rht;
          else if (binaryExpr->op == ">=")
            return lft >= rht;
          throw std::runtime_error("unknown operator");
        },
        left, right);
  }
  throw std::runtime_error("unknown AST node");
}

/*
  Evaluates all AST nodes and returns the result of the last one
*/
void Interpreter::interpret() {
  for (const auto &node : nodes) {
    evaluate(node.get());
  }
}