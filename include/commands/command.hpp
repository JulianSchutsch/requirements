#pragma once

#include <string>
#include <memory>

namespace commands {
  class ICommand;

  std::unique_ptr<ICommand> assembleFromString(const std::string& str);
}