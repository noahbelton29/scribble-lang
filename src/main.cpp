#include "lexer.h"
#include "token_printer.h"

int main() {
  Lexer lexer("const test = 123;");
  printTokens(lexer.tokenise());
  return 0;
}