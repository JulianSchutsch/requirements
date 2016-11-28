#pragma once

#include <vector>
#include <string>

namespace req {
  
  class Status;
  
  void processCommand(Status& status, const std::vector<std::string>& commands);
  
}