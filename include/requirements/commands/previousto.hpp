#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {

    inline void PreviousTo_Operation(NodePtr element, NodePtr anchor) {
      element->setPreviousTo(anchor);
    }
    
    using PreviousTo = internal::AnchoredMove<&PreviousTo_Operation>;
  }
}