#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    void Command_Up_Operation(::requirements::NodePtr node) {
      node->up();
    }
    
    using Command_Up = internal::Move<Command_Up_Operation>;
  }
}