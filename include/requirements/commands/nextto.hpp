#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {

    inline void NextTo_Operation(NodePtr element, NodePtr anchor) {
      element->setNextTo(anchor);
    }
    
    using NextTo = internal::AnchoredMove<&NextTo_Operation>;
  }
}