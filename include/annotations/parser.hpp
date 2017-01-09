#pragma once

#include <map>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"

#include "annotations/sections.hpp"
#include "annotations/errors.hpp"
#include "annotations/shortcuts.hpp"
#include "annotations/requirements.hpp"

namespace annotations {
  
  struct ParserResult {
    Errors errors;
    Sections sections;
    Shortcuts shortcuts;
    Requirements requirements;
  };
  
  bool parse(::requirements::IStorage& storage, ParserResult& result);
}