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
    std::cerr << "usage: scribble <file> [--tokens] [--ast]\n";
    return 1;
  }

  bool printTokensFlag = false;
  bool printASTFlag = false;
  bool noEvalutate = false;
  std::string filePath;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--tokens")
      printTokensFlag = true;
    else if (arg == "--ast")
      printASTFlag = true;
    else if (arg == "--no-evaluate")
      noEvalutate = true;
    else
      filePath = arg;
  }

  if (filePath.empty()) {
    std::cerr << "usage: scribble <file> [--tokens] [--ast]\n";
    return 1;
  }

  const std::string fileContents = readFile(filePath);
  Lexer lexer(fileContents);
  std::vector<Token> tokens = lexer.tokenise();

  if (printTokensFlag) {
    std::cout << "TOKENS:\n";
    printTokens(tokens);
    std::cout << "\n";
  }

  try {
    if (!noEvalutate) {
      Parser parser(tokens);
      auto nodes = parser.parse();

      if (printASTFlag) {
        std::cout << "AST:\n";
        printAST(nodes);
        std::cout << "\n";
      }

      Interpreter interpreter(nodes);
      interpreter.interpret();
    }
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}