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

static void printLine(int indent, const std::string &label) {
  std::cout << indentString(indent) << label << std::endl;
}

void printNode(const ASTNode *node, int indent) {
  if (auto *decl = dynamic_cast<const VarDecl *>(node)) {
    printLine(indent, "VarDecl: " + decl->name);
    printNode(decl->value.get(), indent + 1);
  } else if (auto *decl = dynamic_cast<const ConstDecl *>(node)) {
    printLine(indent, "ConstDecl: " + decl->name);
    printNode(decl->value.get(), indent + 1);
  } else if (auto *decl = dynamic_cast<const AssignStmt *>(node)) {
    printLine(indent, "AssignStmt: " + decl->name);
    printNode(decl->value.get(), indent + 1);
  } else if (auto *stmt = dynamic_cast<const IfStmt *>(node)) {
    printLine(indent, "IfStmt");
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