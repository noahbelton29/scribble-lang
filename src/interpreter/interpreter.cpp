#include "interpreter.h"
#include "ast.h"
#include "error.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

/*
  Constructs an Interpreter with a reference to the parsed AST nodes
*/
Interpreter::Interpreter(const std::vector<std::unique_ptr<ASTNode>> &nodes)
    : nodes(nodes) {
  globalEnv = std::make_shared<Environment>();
  env = globalEnv;
}

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
    env->define(decl->name, value, false);
    return value;
  }
  if (auto *decl = dynamic_cast<const ConstDecl *>(node)) {
    Value value = evaluate(decl->value.get());
    env->define(decl->name, value, true);
    return value;
  }
  if (auto *ident = dynamic_cast<const Identifier *>(node)) {
    return env->get(ident->name);
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
    Value value = evaluate(stmt->value.get());
    env->set(stmt->name, value);
    return value;
  }
  if (auto *stmt = dynamic_cast<const IfStmt *>(node)) {
    Value condition = evaluate(stmt->condition.get());
    if (auto *b = std::get_if<bool>(&condition)) {
      auto previous = env;
      env = std::make_shared<Environment>(env);
      if (*b) {
        for (const auto &node : stmt->body)
          evaluate(node.get());
      } else {
        for (const auto &node : stmt->elseBody)
          evaluate(node.get());
      }
      env = previous;
    } else {
      throw ScribbleError("interpreter", "if condition must be a boolean",
                          stmt->line, stmt->column);
    }
    return condition;
  }
  if (auto *stmt = dynamic_cast<const WhileStmt *>(node)) {
    Value condition = Value{false};
    while (true) {
      condition = evaluate(stmt->condition.get());
      if (auto *b = std::get_if<bool>(&condition)) {
        if (!*b)
          break;
        auto previous = env;
        env = std::make_shared<Environment>(env);
        for (const auto &node : stmt->body)
          evaluate(node.get());
        env = previous;
      } else {
        throw ScribbleError("interpreter", "while condition must be a boolean",
                            stmt->line, stmt->column);
      }
    }
    return condition;
  }
  if (auto *decl = dynamic_cast<const FuncDecl *>(node)) {
    if (functions.count(decl->name))
      throw ScribbleError("interpreter",
                          "function '" + decl->name + "' already declared",
                          decl->line, decl->column);
    functions[decl->name] = decl;
    return Value{false};
  }
  if (auto *call = dynamic_cast<const FuncCall *>(node)) {
    auto iterator = functions.find(call->name);
    if (iterator == functions.end())
      throw ScribbleError("interpreter",
                          "undefined function '" + call->name + "'", call->line,
                          call->column);
    const FuncDecl *decl = iterator->second;

    if (call->args.size() != decl->params.size())
      throw ScribbleError("interpreter",
                          "function '" + call->name + "' expects " +
                              std::to_string(decl->params.size()) +
                              " arguments but got " +
                              std::to_string(call->args.size()),
                          call->line, call->column);

    auto callEnv = std::make_shared<Environment>(globalEnv);
    for (size_t i = 0; i < call->args.size(); i++) {
      Value arg = evaluate(call->args[i].get());
      callEnv->define(decl->params[i], arg, false);
    }

    auto previous = env;
    env = callEnv;
    for (const auto &node : decl->body)
      evaluate(node.get());
    env = previous;
    return Value{false};
  }

  /*
    Evaluates both sides of a binary expression and applies the operator
  */
  if (auto *binaryExpr = dynamic_cast<const BinaryExpr *>(node)) {
    Value left = evaluate(binaryExpr->left.get());
    Value right = evaluate(binaryExpr->right.get());

    return std::visit(
        [&](auto lft, auto rht) -> Value {
          using L = decltype(lft);
          using R = decltype(rht);
          if constexpr (std::is_same_v<L, R>) {
            if (binaryExpr->op == "==")
              return lft == rht;
            if (binaryExpr->op == "!=")
              return lft != rht;
          }
          if constexpr (!std::is_arithmetic_v<L> || !std::is_arithmetic_v<R> ||
                        std::is_same_v<L, bool> || std::is_same_v<R, bool>)
            throw ScribbleError("interpreter",
                                "type mismatch: cannot apply '" +
                                    binaryExpr->op + "' to incompatible types",
                                binaryExpr->line, binaryExpr->column);
          else if (binaryExpr->op == "+")
            return lft + rht;
          else if (binaryExpr->op == "-")
            return lft - rht;
          else if (binaryExpr->op == "*")
            return lft * rht;
          else if (binaryExpr->op == "/")
            return lft / rht;
          else if (binaryExpr->op == "<")
            return lft < rht;
          else if (binaryExpr->op == ">")
            return lft > rht;
          else if (binaryExpr->op == "<=")
            return lft <= rht;
          else if (binaryExpr->op == ">=")
            return lft >= rht;
          throw ScribbleError("interpreter",
                              "unknown operator '" + binaryExpr->op + "'",
                              binaryExpr->line, binaryExpr->column);
        },
        left, right);
  }
  throw ScribbleError("interpreter", "unknown AST node", 0, 0);
}

/*
  Evaluates all AST nodes and returns the result of the last one
*/
void Interpreter::interpret() {
  for (const auto &node : nodes) {
    evaluate(node.get());
  }
}