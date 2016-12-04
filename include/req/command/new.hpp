#pragma once

#include <vector>
#include <string>

namespace req {
  class Status;
  namespace command {
    void processCommand_new(Status& status, const std::vector<std::string>& parameters);
  }
}