#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    void Command_LastOf_Operation(::requirements::NodePtr element, ::requirements::NodePtr anchor) {
      element->setLastOf(anchor);
    }
    
    using Command_LastOf = internal::AnchoredMove<&Command_LastOf_Operation>;
    
  }
}