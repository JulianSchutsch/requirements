#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    
    inline void Down_Operation(::requirements::NodePtr node) {
      node->down();
    }
    
    using Down = internal::Move<&Down_Operation>;
  }
}