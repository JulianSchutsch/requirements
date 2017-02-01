#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    inline void Up_Operation(::requirements::NodePtr node) {
      node->up();
    }
    
    using Up = internal::Move<Up_Operation>;
  }
}