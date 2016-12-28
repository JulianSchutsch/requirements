#pragma once

#include <string>
#include <vector>

namespace req {
  class Status;
  namespace command {
    void processCommand_up(Status& status, const std::vector<std::string>& parameters);
    void processCommand_down(Status& status, const std::vector<std::string>& parameters);
  }
}