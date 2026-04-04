#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>

/*
  Represents a Scribble error with phase, message, line and column information
*/
struct ScribbleError : std::runtime_error {
  std::size_t line;
  std::size_t column;
  std::string phase;
  std::string message;

  ScribbleError(const std::string &phase, const std::string &message,
                std::size_t line, std::size_t column)
      : std::runtime_error(message), line(line), column(column), phase(phase),
        message(message) {}
};
