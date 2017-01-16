#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    void Command_FirstOf_Operation(::requirements::NodePtr element, ::requirements::NodePtr anchor) {
      element->setFirstOf(anchor);
    }
    
    using Command_FirstOf = internal::AnchoredMove<&Command_FirstOf_Operation>;
    
  }
}