#include "ast.h"
#include "ast_printer.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "token_printer.h"

#include <iostream>
#include <string>

int main() {
  Lexer lexer("var test = 582 + 2.52;");
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
  Value result = interpreter.interpret();
  std::visit([](auto visitor) { std::cout << "Result: " << visitor << "\n"; },
             result);
  return 0;
}