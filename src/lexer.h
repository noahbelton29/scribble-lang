#pragma once

#include <cstddef>
#include <string>
#include <vector>

/*
  Token types represent the category of each scanned token,
  used by the lexer to classify input during tokenisation.
*/
enum class TokenType {
  // Literals
  Number,
  Identifier,

  // Keywords
  Var,
  Const,

  // Arithmetic
  Plus,
  Minus,
  Slash,
  Star,

  // Operators
  Equals,

  // Punctuation
  Semicolon,
  Colon,

  // Other
  Unknown,
  EndOfFile
};

/*
  A Token holds a Token type and its raw string value from the source input.
*/
struct Token {
  TokenType type{TokenType::Unknown};
  std::string value{};
  std::size_t line{1};
  std::size_t column{1};
};

/*
  The Lexer takes a string of source code and breaks it into tokens.
*/
class Lexer {
public:
  explicit Lexer(std::string input);
  std::vector<Token> tokenise();

private:
  std::string input{};
  std::size_t position{0};
  std::size_t line{1};
  std::size_t column{1};

  char currentChar() const;
  void advance();

  void skipWhitespace();

  Token number();
  Token identifier();
  Token makeToken(TokenType type, std::string value) const;
};