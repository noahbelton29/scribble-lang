#pragma once

#include <cstdint>
#include <memory>
#include <string>

/*
  Base node for all AST nodes
*/
struct ASTNode {
  virtual ~ASTNode() = default;
};

/*
  Represents a reference to a variable by name
*/
struct Identifier : ASTNode {
  std::string name;
};

/*
  Represents a binary operation e.g. 1 + 2
*/
struct BinaryExpr : ASTNode {
  std::unique_ptr<ASTNode> left;
  std::string op;
  std::unique_ptr<ASTNode> right;
};

/*
  Represents a numeric literal value in the source code
*/
struct NumberLiteral : ASTNode {
  uint64_t value;
};

/*
  Represents a variable declaration of the form: var <name> = <value>
*/
struct VarDecl : ASTNode {
  std::string name;
  std::unique_ptr<ASTNode> value;
};