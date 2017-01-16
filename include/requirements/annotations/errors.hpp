#pragma once

#include <string>

#include "common.hpp"

namespace annotations {
  
  using Errors = Common<std::string>;
  using ErrorsBuilder = CommonBuilder<std::string>;

}