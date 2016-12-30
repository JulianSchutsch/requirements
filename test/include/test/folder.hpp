#pragma once

#include <string>

namespace test {

  class UniqueFolder final {
  private:
    std::string name;
  public:
    std::string getName() const noexcept { return name; }
    UniqueFolder();
    ~UniqueFolder();
  };

}