#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    void LastOf_Operation(NodePtr element, NodePtr anchor) {
      element->setLastOf(anchor);
    }
    
    using LastOf = internal::AnchoredMove<&LastOf_Operation>;
    
  }
}