#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {
    
    void FirstOf_Operation(::requirements::NodePtr element, ::requirements::NodePtr anchor) {
      element->setFirstOf(anchor);
    }
    
    using FirstOf = internal::AnchoredMove<&FirstOf_Operation>;
    
  }
}