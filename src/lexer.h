#pragma once

#include <cstddef>
#include <string>
#include <vector>

enum class TokenType { Number, Identifier, Plus, Unknown, EndOfFile };

struct Token {
  TokenType type{TokenType::Unknown};
  std::string value{};
};

class Lexer {
public:
  explicit Lexer(std::string input);
  std::vector<Token> tokenise();

private:
  std::string input{};
  std::size_t position{0};

  char currentChar() const;
  void advance();

  void skipWhitespace();

  Token number();
  Token identifier();
  Token makeToken(TokenType type, std::string value) const;
};