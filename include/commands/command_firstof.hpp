#pragma once

#include "commands/move.hpp"

namespace commands {
  
  void Command_FirstOf_Operation(::requirements::Id element, ::requirements::Id anchor) {
    element->setFirstOf(anchor);
  };
  
  using Command_FirstOf = AnchoredMove<&Command_FirstOf_Operation>;
  
}