#pragma once

#include <string>

#include "requirements/annotations/internal/common.hpp"

namespace requirements {
  namespace annotations {
    
    using Shortcuts = internal::Common<std::string>;
    using ShortcutsBuilder = internal::CommonBuilder<std::string>;
    
  }
}