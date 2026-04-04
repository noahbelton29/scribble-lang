#include "parser.h"

/*
  Parses a constant variable declaration of the form: const <name> = <expr>;
*/
std::unique_ptr<ConstDecl> Parser::parseConstDecl() {
  expect(TokenType::Const);

  Token name = expect(TokenType::Identifier);
  expect(TokenType::Equals);

  auto value = parseExpression();
  expect(TokenType::Semicolon);

  auto constDecl = std::make_unique<ConstDecl>();
  constDecl->name = name.value;
  constDecl->line = name.line;
  constDecl->column = name.column;
  constDecl->value = std::move(value);
  return constDecl;
}

/*
  Parses a variable declaration of the form: var <name> = <expr>;
*/
std::unique_ptr<VarDecl> Parser::parseVarDecl() {
  expect(TokenType::Var);

  Token name = expect(TokenType::Identifier);
  expect(TokenType::Equals);

  auto value = parseExpression();
  expect(TokenType::Semicolon);

  auto varDecl = std::make_unique<VarDecl>();
  varDecl->name = name.value;
  varDecl->line = name.line;
  varDecl->column = name.column;
  varDecl->value = std::move(value);
  return varDecl;
}