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
    std::unique_ptr<Errors> errors;
    std::unique_ptr<Sections> sections;
    std::unique_ptr<Shortcuts> shortcuts;
    std::unique_ptr<Requirements> requirements;
  };
  
  bool parse(::requirements::IStorage& storage, ParserResult& result);
}