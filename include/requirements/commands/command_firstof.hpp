#pragma once

#include "move.hpp"

namespace commands {
  
  void Command_FirstOf_Operation(::requirements::NodePtr element, ::requirements::NodePtr anchor) {
    element->setFirstOf(anchor);
  }
  
  using Command_FirstOf = AnchoredMove<&Command_FirstOf_Operation>;
  
}