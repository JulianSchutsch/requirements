#pragma once

#include <string>

namespace annotations {
  
  class ShortCut {
  private:
    std::string shortCut;
  public:
    const std::string& getShortcut() const { return shortCut; }
  };
  
}