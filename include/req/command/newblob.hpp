#pragma once

#include <string>
#include <vector>

namespace req {
  class Status;
  namespace command {
    void processCommand_newBlob(Status& status, const std::vector<std::string>& parameters);
  }
}