#include "ast_printer.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "token_printer.h"

#include <iostream>

int main() {
  Lexer lexer("var test = 582 + 2;");
  std::vector<Token> tokens = lexer.tokenise();

  std::cout << "TOKENS: " << std::endl;
  printTokens(tokens);
  std::cout << std::endl;

  std::cout << "AST: " << std::endl;
  Parser parser(tokens);
  auto nodes = parser.parse();
  printAST(nodes);
  std::cout << std::endl;

  Interpreter interpreter(nodes);
  uint64_t result = interpreter.interpret();
  std::cout << "Result: " << result << "\n";
  return 0;
}