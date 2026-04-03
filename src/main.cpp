#include "ast.h"
#include "ast_printer.h"
#include "file_utils.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "token_printer.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: scribble <file>\n";
    return 1;
  }

  const std::string fileContents = readFile(argv[1]);

  Lexer lexer(fileContents);
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