#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

using Value = std::variant<int64_t, double, bool, std::string>;

/*
  Base node for all AST nodes
*/
struct ASTNode {
  virtual ~ASTNode() = default;
  std::size_t line{0};
  std::size_t column{0};
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
  int64_t value;
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

/*
  Represents an if statement of the form: if <expr> { ... }
*/
struct IfStmt : ASTNode {
  std::unique_ptr<ASTNode> condition;
  std::vector<std::unique_ptr<ASTNode>> body;
  std::vector<std::unique_ptr<ASTNode>> elseBody;
};

/*
  Represents an if statement of the form: while <expr> { ... }
*/
struct WhileStmt : ASTNode {
  std::unique_ptr<ASTNode> condition;
  std::vector<std::unique_ptr<ASTNode>> body;
};

/*
  Represents a function declaration of the form: func name(p1, p2) { ... }
*/
struct FuncDecl : ASTNode {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<ASTNode>> body;
};

/*
  Represents a function call of the form: name();
*/
struct FuncCall : ASTNode {
  std::string name;
  std::vector<std::unique_ptr<ASTNode>> args;
};

/*
  Represents a return statement of the form: ret <expr>;
*/
struct ReturnStmt : ASTNode {
  std::unique_ptr<ASTNode> value;
};