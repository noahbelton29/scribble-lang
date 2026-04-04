#include "error_reporter.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
  Splits a source string into a vector of lines
*/
static std::vector<std::string> splitLines(const std::string &source) {
  std::vector<std::string> lines;
  std::istringstream stream(source);
  std::string line;
  while (std::getline(stream, line)) {
    lines.push_back(line);
  }
  return lines;
}

/*
  Returns a string of n repeated characters
*/
static std::string repeat(char c, std::size_t n) { return std::string(n, c); }

/*
  Prints a beautiful ariadne-style error report to stderr
*/
void reportError(const ScribbleError &error, const std::string &source,
                 const std::string &filename) {
  const std::string red = "\033[1;31m";
  const std::string cyan = "\033[1;36m";
  const std::string yellow = "\033[1;33m";
  const std::string bold = "\033[1m";
  const std::string reset = "\033[0m";

  auto lines = splitLines(source);

  // error: message
  std::cerr << red << bold << "error" << reset << bold << "[" << error.phase
            << "]" << reset << ": " << bold << error.message << reset << "\n";

  // filename:line:column
  std::cerr << cyan << "  --> " << reset << filename << ":" << error.line << ":"
            << error.column << "\n";

  if (error.line == 0 || error.line > lines.size()) {
    std::cerr << "\n";
    return;
  }

  const std::string &sourceLine = lines[error.line - 1];
  std::string lineNum = std::to_string(error.line);
  std::string padding = repeat(' ', lineNum.size());

  // blank gutter line
  std::cerr << cyan << " " << padding << " |" << reset << "\n";

  // source line with line number
  std::cerr << cyan << " " << lineNum << " | " << reset << sourceLine << "\n";

  // caret line
  std::size_t caretCol = error.column > 0 ? error.column - 1 : 0;
  std::string caretPad = repeat(' ', caretCol);

  // try to highlight the whole token, find length of word at column
  std::size_t tokenLen = 1;
  if (caretCol < sourceLine.size()) {
    std::size_t end = caretCol;
    while (end < sourceLine.size() &&
           (std::isalnum(static_cast<unsigned char>(sourceLine[end])) ||
            sourceLine[end] == '_')) {
      end++;
    }
    tokenLen = std::max<std::size_t>(1, end - caretCol);
  }

  std::cerr << cyan << " " << padding << " | " << reset << yellow << caretPad
            << repeat('^', tokenLen) << reset << "\n";

  // blank closing gutter line
  std::cerr << cyan << " " << padding << " |" << reset << "\n\n";
}
