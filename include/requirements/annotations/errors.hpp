#pragma once

#include <string>

#include "requirements/annotations/internal/common.hpp"

namespace requirements {
  namespace annotations {
    
    using Errors = internal::Common<std::string>;
    using ErrorsBuilder = internal::CommonBuilder<std::string>;
    
  }
}