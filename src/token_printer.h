#pragma once

#include "lexer.h"

#include <string>
#include <vector>

std::string tokenTypeName(TokenType type);
void printTokens(const std::vector<Token> &tokens);