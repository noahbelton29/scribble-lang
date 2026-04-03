#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token_printer.h"

#include <iostream>

int main() {
  Lexer lexer("var test = 18446744073709551615;");
  std::vector<Token> tokens = lexer.tokenise();
  printTokens(tokens);
  Parser parser(tokens);
  auto nodes = parser.parse();

  for (const auto &node : nodes) {
    if (auto *decl = dynamic_cast<VarDecl *>(node.get())) {
      auto *num = dynamic_cast<NumberLiteral *>(decl->value.get());
      std::cout << "VarDecl: " << decl->name << " = " << num->value << "\n";
    }
  }
  return 0;
}