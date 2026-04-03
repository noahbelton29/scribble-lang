#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <variant>

using Value = std::variant<uint64_t, double, bool, std::string>;

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
  Represents a binary operation e.g. 1 + 2;
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
  Represents a floating point literal value in the source code
*/
struct FloatLiteral : ASTNode {
  double value;
};

/*
  Represents a boolean literal value in the source code
*/
struct BoolLiteral : ASTNode {
  bool value;
};

/*
  Represents a string literal value in the source code
*/
struct StringLiteral : ASTNode {
  std::string value;
};

/*
  Represents a variable declaration of the form: var <name> = <value>;
*/
struct VarDecl : ASTNode {
  std::string name;
  std::unique_ptr<ASTNode> value;
};

/*
  Represents a constant variable declaration of the form: const <name> =
  <value>;
*/
struct ConstDecl : ASTNode {
  std::string name;
  std::unique_ptr<ASTNode> value;
};

/*
  Represents a print statement of the form: print(); or println();
*/
struct PrintStmt : ASTNode {
  std::unique_ptr<ASTNode> value;
  bool newline{false};
};

/*
  Represents reassignment of the form: x = <expr>;
*/
struct AssignStmt : ASTNode {
  std::string name;
  std::unique_ptr<ASTNode> value;
};