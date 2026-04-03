#include "lexer.h"

#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>

/*
  Maps keyword literals to Token types
*/
static const std::unordered_map<std::string, TokenType> keywords = {
    {"var", TokenType::Var},     {"const", TokenType::Const},
    {"true", TokenType::True},   {"false", TokenType::False},
    {"print", TokenType::Print}, {"println", TokenType::Println}};

/*
  Constructs a Lexer object with the input string to tokenise
 */
Lexer::Lexer(std::string input) : input(std::move(input)) {}

/*
  Breaks the input into tokens, skipping spaces
  Handles numbers, identifiers, symbols, and unknown characters
  Appends an EndOfFile token at the end
*/
std::vector<Token> Lexer::tokenise() {
  std::vector<Token> tokens;

  // Iterate through the current character until the end of input
  while (currentChar() != '\0') {
    // Skip whitespaces
    if (std::isspace(static_cast<unsigned char>(currentChar()))) {
      skipWhitespace();
      continue;
    }

    // Numbers
    if (std::isdigit(static_cast<unsigned char>(currentChar()))) {
      tokens.push_back(number());
      continue;
    }

    // Strings
    if (currentChar() == '"') {
      tokens.push_back(string());
      continue;
    }

    if (std::isalpha(static_cast<unsigned char>(currentChar()))) {
      tokens.push_back(identifier());
      continue;
    }

    // Symbols
    switch (currentChar()) {
    case '+':
      tokens.push_back(makeToken(TokenType::Plus, "+"));
      advance();
      break;
    case '-':
      tokens.push_back(makeToken(TokenType::Minus, "-"));
      advance();
      break;
    case '/':
      tokens.push_back(makeToken(TokenType::Slash, "/"));
      advance();
      break;
    case '*':
      tokens.push_back(makeToken(TokenType::Star, "*"));
      advance();
      break;
    case '=':
      tokens.push_back(makeToken(TokenType::Equals, "="));
      advance();
      break;
    case ':':
      tokens.push_back(makeToken(TokenType::Colon, ":"));
      advance();
      break;
    case ';':
      tokens.push_back(makeToken(TokenType::Semicolon, ";"));
      advance();
      break;
    case '(':
      tokens.push_back(makeToken(TokenType::LParen, "("));
      advance();
      break;
    case ')':
      tokens.push_back(makeToken(TokenType::RParen, ")"));
      advance();
      break;
    default:
      tokens.push_back(
          makeToken(TokenType::Unknown, std::string(1, currentChar())));
      advance();
      break;
    }
  }

  tokens.push_back(makeToken(TokenType::EndOfFile, ""));
  return tokens;
}

/*
  Constructs a token with a given TokenType and value
*/
Token Lexer::makeToken(TokenType type, std::string value) const {
  return Token{type, std::move(value), line, column};
}

/*
  Collects all characters within the quotes
  Returns a constructed string from the characters
*/
Token Lexer::string() {
  std::string createdString;
  if (currentChar() == '"') {
    advance();
    while (currentChar() != '"' && currentChar() != '\0') {
      createdString += input[position];
      advance();
    }
    if (currentChar() == '\0')
      throw std::runtime_error("unterminated string literal on line " +
                               std::to_string(line));
    advance();
  }
  return makeToken(TokenType::StringLiteral, createdString);
}

/*
  Collects digits and creates a string from them
  Returns a constructed token from the string it creates
*/
Token Lexer::number() {
  std::string number;
  while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
    number += input[position];
    advance();
  }
  if (currentChar() == '.') {
    number += ".";
    advance();
    while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
      number += input[position];
      advance();
    }
    return makeToken(TokenType::FloatLiteral, number);
  }
  return makeToken(TokenType::NumberLiteral, number);
}

/*
  Collects characters and creates a string from them
  Returns a constructed token from the string it creates
*/
Token Lexer::identifier() {
  std::string identifier;
  while (std::isalnum(static_cast<unsigned char>(currentChar()))) {
    identifier += input[position];
    advance();
  }
  auto iterator = keywords.find(identifier);
  if (iterator != keywords.end())
    return makeToken(iterator->second, identifier);
  return makeToken(TokenType::Identifier, identifier);
}

/*
  Skips any whitespaces and advances the position
*/
void Lexer::skipWhitespace() {
  while (currentChar() != '\0' &&
         std::isspace(static_cast<unsigned char>(currentChar()))) {
    advance();
  }
}

/*
  Advance until the position is not less than the input size
*/
void Lexer::advance() {
  if (position < input.size()) {
    if (input[position] == '\n') {
      line++;
      column = 1;
    } else {
      column++;
    }
    position++;
  }
}

/*
  Get the current character in the input
*/
char Lexer::currentChar() const {
  return (position < input.size()) ? input[position] : '\0';
}