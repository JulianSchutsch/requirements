#pragma once

#include "requirements/commands/internal/move.hpp"

namespace requirements {
  namespace commands {

    inline void LevelUp_Operation(::requirements::NodePtr node) {
      node->levelUp();
    }

    using LevelUp = internal::Move<LevelUp_Operation>;
  }
}