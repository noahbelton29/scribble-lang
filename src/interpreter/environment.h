#pragma once

#include "ast.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

/*
  Represents a single scope, holding variables and a pointer to the
  enclosing scope. Scopes form a chain from innermost to global.
*/
struct Environment {
  std::unordered_map<std::string, Value> variables{};
  std::unordered_set<std::string> constants{};
  std::shared_ptr<Environment> parent{};

  explicit Environment(std::shared_ptr<Environment> parent = nullptr);

  Value get(const std::string &name) const;
  void set(const std::string &name, Value value);
  void define(const std::string &name, Value value, bool isConst);
};
