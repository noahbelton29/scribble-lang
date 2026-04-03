#pragma once

#include "ast.h"

#include <memory>
#include <vector>

void printNode(const ASTNode *node, int indent = 0);
void printAST(const std::vector<std::unique_ptr<ASTNode>> &nodes);
