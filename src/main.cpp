#include "lexer.h"
#include <iostream>
#include <vector>

int main() {
  Lexer lexer("test + 123");
  std::vector<Token> tokens = lexer.tokenise();

  for (const auto &token : tokens) {
    std::cout << "Token type: ";
    switch (token.type) {
    case TokenType::Number:
      std::cout << "Number";
      break;
    case TokenType::Identifier:
      std::cout << "Identifier";
      break;
    case TokenType::Plus:
      std::cout << "Plus";
      break;
    case TokenType::Unknown:
      std::cout << "Unknown";
      break;
    case TokenType::EndOfFile:
      std::cout << "EndOfFile";
      break;
    }
    std::cout << ", value: \"" << token.value << "\"\n";
  }

  return 0;
}