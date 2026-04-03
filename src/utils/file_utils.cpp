#include "file_utils.h"

#include <fstream>
#include <sstream>
#include <string>

/*
  Reads contents of a given file and returns it as a string
*/
std::string readFile(const std::string &filePath) {
  std::ifstream file(filePath);
  std::ostringstream content;
  content << file.rdbuf();
  return content.str();
}