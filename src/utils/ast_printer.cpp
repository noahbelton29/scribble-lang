#include "ast_printer.h"
#include "ast.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*
  Returns a string of spaces for the given indentation level
*/
static std::string indentString(int indent) {
  return std::string(indent * 2, ' ');
}

/*
  Prints a string with an indented label
*/
static void printLine(int indent, const std::string &label) {
  std::cout << indentString(indent) << label << std::endl;
}

/*
  Prints an AST Node in a readable format
*/
void printNode(const ASTNode *node, int indent) {
  if (auto *decl = dynamic_cast<const VarDecl *>(node)) {
    printLine(indent, "VarDecl: " + decl->name);
    printNode(decl->value.get(), indent + 1);
  } else if (auto *decl = dynamic_cast<const ConstDecl *>(node)) {
    printLine(indent, "ConstDecl: " + decl->name);
    printNode(decl->value.get(), indent + 1);
  } else if (auto *decl = dynamic_cast<const FuncDecl *>(node)) {
    std::string params;
    for (size_t i = 0; i < decl->params.size(); i++) {
      params += decl->params[i];
      if (i + 1 < decl->params.size())
        params += ", ";
    }
    printLine(indent, "FuncDecl: " + decl->name + "(" + params + ")");
    for (const auto &node : decl->body)
      printNode(node.get(), indent + 1);
  } else if (auto *call = dynamic_cast<const FuncCall *>(node)) {
    printLine(indent, "FuncCall: " + call->name);
    for (const auto &arg : call->args)
      printNode(arg.get(), indent + 1);
  } else if (auto *decl = dynamic_cast<const AssignStmt *>(node)) {
    printLine(indent, "AssignStmt: " + decl->name);
    printNode(decl->value.get(), indent + 1);
  } else if (auto *stmt = dynamic_cast<const IfStmt *>(node)) {
    printLine(indent, "IfStmt");
    printNode(stmt->condition.get(), indent + 1);
    for (const auto &node : stmt->body)
      printNode(node.get(), indent + 1);
    if (!stmt->elseBody.empty()) {
      printLine(indent, "ElseBody");
      for (const auto &node : stmt->elseBody)
        printNode(node.get(), indent + 1);
    }
  } else if (auto *stmt = dynamic_cast<const WhileStmt *>(node)) {
    printLine(indent, "WhileStmt");
    printNode(stmt->condition.get(), indent + 1);
    for (const auto &node : stmt->body)
      printNode(node.get(), indent + 1);
  } else if (auto *expr = dynamic_cast<const BinaryExpr *>(node)) {
    printLine(indent, "BinaryExpr: " + expr->op);
    printNode(expr->left.get(), indent + 1);
    printNode(expr->right.get(), indent + 1);
  } else if (auto *lit = dynamic_cast<const NumberLiteral *>(node)) {
    printLine(indent, "NumberLiteral: " + std::to_string(lit->value));
  } else if (auto *lit = dynamic_cast<const StringLiteral *>(node)) {
    printLine(indent, "StringLiteral: " + lit->value);
  } else if (auto *lit = dynamic_cast<const FloatLiteral *>(node)) {
    printLine(indent, "FloatLiteral: " + std::to_string(lit->value));
  } else if (auto *lit = dynamic_cast<const BoolLiteral *>(node)) {
    printLine(indent,
              std::string("BoolLiteral: ") + (lit->value ? "true" : "false"));
  } else if (auto *ident = dynamic_cast<const Identifier *>(node)) {
    printLine(indent, "Identifier: " + ident->name);
  } else if (auto *stmt = dynamic_cast<const PrintStmt *>(node)) {
    printLine(indent, stmt->newline ? "Println" : "Print");
    printNode(stmt->value.get(), indent + 1);
  }
}

/*
  Recursively prints nodes to visualise the AST
*/
void printAST(const std::vector<std::unique_ptr<ASTNode>> &nodes) {
  for (const auto &node : nodes) {
    printNode(node.get());
  }
}