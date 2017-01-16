#pragma once

#include "move.hpp"

namespace commands {
  
  class Status;
  
  void Command_Up_Operation(::requirements::NodePtr node) {
    node->up();
  }
  
  using Command_Up = Move<Command_Up_Operation>;
}