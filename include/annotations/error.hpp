#pragma once

#include <string>

namespace annotations {
  class Error {
  private:
    std::string error;
  public:
    const std::string getError() const { return error; }
  };
}