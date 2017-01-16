#pragma once

#include "move.hpp"

namespace commands {
  
  void Command_LastOf_Operation(::requirements::NodePtr element, ::requirements::NodePtr anchor) {
    element->setLastOf(anchor);
  }
  
  using Command_LastOf = AnchoredMove<&Command_LastOf_Operation>;
  
}