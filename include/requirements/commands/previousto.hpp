#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    void PreviousTo_Operation(NodePtr element, NodePtr anchor) {
      element->setNextTo(anchor);
    }
    
    using PreviousTo = internal::AnchoredMove<&NextTo_Operation>;
  }
}