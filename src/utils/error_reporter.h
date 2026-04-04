#pragma once
#include "error.h"
#include <string>

/*
  Prints a beautiful ariadne inspired error report to stderr
*/
void reportError(const ScribbleError &error, const std::string &source,
                 const std::string &filename);
