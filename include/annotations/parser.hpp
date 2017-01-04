#pragma once

#include <map>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"

#include "annotations/sections.hpp"
#include "annotations/errors.hpp"
#include "annotations/shortcuts.hpp"

namespace annotations {
  
  class Error;
  
  struct ParserResult {
    Errors errors;
    Sections sections;
    Shortcuts shortcuts;
  };
  
  bool parse(::requirements::IStorage& storage, ParserResult& result);
}