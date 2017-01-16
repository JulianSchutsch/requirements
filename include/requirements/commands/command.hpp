#pragma once

#include <string>
#include <memory>

namespace requirements {
  class ICommand;
  namespace commands {
  
    std::unique_ptr<ICommand> assembleFromString(const std::string& str);
  }
}