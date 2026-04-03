#include "token_printer.h"

#include <iostream>

/*
  Maps token types to strings
*/
std::string tokenTypeName(TokenType type) {
  switch (type) {
  case TokenType::NumberLiteral:
    return "Number";
  case TokenType::FloatLiteral:
    return "Float";
  case TokenType::StringLiteral:
    return "String";
  case TokenType::Identifier:
    return "Identifier";
  case TokenType::Var:
    return "Var";
  case TokenType::Const:
    return "Const";
  case TokenType::Plus:
    return "Plus";
  case TokenType::Slash:
    return "Slash";
  case TokenType::Star:
    return "Star";
  case TokenType::Minus:
    return "Minus";
  case TokenType::Equals:
    return "Equals";
  case TokenType::Semicolon:
    return "Semicolon";
  case TokenType::Colon:
    return "Colon";
  case TokenType::True:
    return "True";
  case TokenType::False:
    return "False";
  case TokenType::Unknown:
    return "Unknown";
  case TokenType::EndOfFile:
    return "EndOfFile";
  default:
    return "Unknown token";
  }
}

/*
  Prints a vector of tokens in a readable format
*/
void printTokens(const std::vector<Token> &tokens) {
  for (const auto &token : tokens) {
    std::cout << "Token type: " << tokenTypeName(token.type) << ", value: \""
              << token.value << "\"\n";
  }
}