#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {

    inline void LevelDown_Operation(::requirements::NodePtr node) {
      node->levelDown();
    }

    using LevelDown = internal::Move<LevelDown_Operation>;
  }
}