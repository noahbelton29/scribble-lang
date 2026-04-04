#include "environment.h"
#include "error.h"

/*
  Constructs an Environment with an optional parent scope
*/
Environment::Environment(std::shared_ptr<Environment> parent)
    : parent(std::move(parent)) {}

/*
  Searches the current scope then walks up the chain until the variable
  is found, throwing if it does not exist in any enclosing scope
*/
Value Environment::get(const std::string &name) const {
  auto it = variables.find(name);
  if (it != variables.end())
    return it->second;
  if (parent)
    return parent->get(name);
  throw ScribbleError("interpreter", "undefined variable '" + name + "'", 0, 0);
}

/*
  Searches the current scope then walks up the chain until the variable
  is found and updates it, throwing if it is const or does not exist
*/
void Environment::set(const std::string &name, Value value) {
  auto it = variables.find(name);
  if (it != variables.end()) {
    if (constants.count(name))
      throw ScribbleError("interpreter", "cannot reassign const '" + name + "'",
                          0, 0);
    it->second = value;
    return;
  }
  if (parent) {
    parent->set(name, value);
    return;
  }
  throw ScribbleError("interpreter", "undefined variable '" + name + "'", 0, 0);
}

/*
  Creates a new variable in the current scope, marking it const if specified
*/
void Environment::define(const std::string &name, Value value, bool isConst) {
  variables[name] = value;
  if (isConst)
    constants.insert(name);
}