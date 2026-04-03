#include "ast_printer.h"
#include "lexer.h"
#include "parser.h"
#include "token_printer.h"

#include <iostream>

int main() {
  Lexer lexer("var test = 582 + test + 2;");
  std::vector<Token> tokens = lexer.tokenise();

  std::cout << "TOKENS: " << std::endl;
  printTokens(tokens);
  std::cout << std::endl;

  std::cout << "AST: " << std::endl;
  Parser parser(tokens);
  auto nodes = parser.parse();
  printAST(nodes);
  std::cout << std::endl;
  return 0;
}