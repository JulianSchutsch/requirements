#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    
    void Command_Down_Operation(::requirements::NodePtr node) {
      node->down();
    }
    
    using Command_Down = internal::Move<&Command_Down_Operation>;
  }
}