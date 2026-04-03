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
  Prints nodes based on their ASTNode type
*/
void printNode(const ASTNode *node, int indent) {
  if (auto *decl = dynamic_cast<const VarDecl *>(node)) {
    std::cout << indentString(indent) << "VarDecl: " << decl->name << std::endl;
    printNode(decl->value.get(), indent + 1);
  } else if (auto *expr = dynamic_cast<const BinaryExpr *>(node)) {
    std::cout << indentString(indent) << "BinaryExpr: " << expr->op
              << std::endl;
    printNode(expr->left.get(), indent + 1);
    printNode(expr->right.get(), indent + 1);
  } else if (auto *lit = dynamic_cast<const NumberLiteral *>(node)) {
    std::cout << indentString(indent) << "NumberLiteral: " << lit->value
              << std::endl;
  } else if (auto *lit = dynamic_cast<const StringLiteral *>(node)) {
    std::cout << indentString(indent) << "StringLiteral: " << lit->value
              << std::endl;
  } else if (auto *lit = dynamic_cast<const FloatLiteral *>(node)) {
    std::cout << indentString(indent) << "FloatLiteral: " << lit->value
              << std::endl;
  } else if (auto *lit = dynamic_cast<const BoolLiteral *>(node)) {
    std::cout << indentString(indent)
              << "BoolLiteral: " << (lit->value ? "true" : "false")
              << std::endl;
  } else if (auto *ident = dynamic_cast<const Identifier *>(node)) {
    std::cout << indentString(indent) << "Identifier: " << ident->name
              << std::endl;
  } else if (auto *stmt = dynamic_cast<const PrintStmt *>(node)) {
    std::cout << indentString(indent) << (stmt->newline ? "Println" : "Print")
              << std::endl;
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