#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    void LastOf_Operation(::requirements::NodePtr element, ::requirements::NodePtr anchor) {
      element->setLastOf(anchor);
    }
    
    using LastOf = internal::AnchoredMove<&LastOf_Operation>;
    
  }
}