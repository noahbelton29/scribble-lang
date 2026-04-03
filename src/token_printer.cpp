#include "token_printer.h"
#include <iostream>

/*
  Maps token types to strings
*/
std::string tokenTypeName(TokenType type) {
  switch (type) {
  case TokenType::Number:
    return "Number";
    break;
  case TokenType::Identifier:
    return "Identifier";
    break;
  case TokenType::Var:
    return "Var";
    break;
  case TokenType::Const:
    return "Const";
    break;
  case TokenType::Plus:
    return "Plus";
    break;
  case TokenType::Slash:
    return "Slash";
    break;
  case TokenType::Star:
    return "Star";
    break;
  case TokenType::Minus:
    return "Minus";
    break;
  case TokenType::Equals:
    return "Equals";
    break;
  case TokenType::Semicolon:
    return "Semicolon";
    break;
  case TokenType::Colon:
    return "Colon";
    break;
  case TokenType::Unknown:
    return "Unknown";
    break;
  case TokenType::EndOfFile:
    return "EndOfFile";
    break;
  default:
    return "Unknown token";
    break;
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