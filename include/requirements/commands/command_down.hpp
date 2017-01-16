#pragma once

#include "move.hpp"

namespace commands {
  
  class Status;
  
  void Command_Down_Operation(::requirements::NodePtr node) {
    node->down();
  }
  
  using Command_Down = Move<&Command_Down_Operation>;
}