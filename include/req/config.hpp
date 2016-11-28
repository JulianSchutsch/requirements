#pragma once

#include <string>
#include <list>

namespace req {
  
  class Config {
  public:
    using Selection = std::list<std::string>;
    std::string folder;
    std::list<Selection> selections;
    bool load();
    bool save();
  };
  
}