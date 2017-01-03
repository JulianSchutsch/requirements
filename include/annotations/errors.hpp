#pragma once

#include <string>

#include "annotations/common.hpp"

namespace annotations {
  
  using Errors = Common<std::string>;
  using ErrorsBuilder = CommonBuilder<std::string>;

}