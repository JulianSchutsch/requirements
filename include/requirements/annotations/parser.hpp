#pragma once

#include <map>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"

#include "sections.hpp"
#include "errors.hpp"
#include "shortcuts.hpp"
#include "requirements.hpp"

namespace requirements {
  namespace annotations {
    
    struct ParserResult {
      std::unique_ptr<Errors> errors;
      std::unique_ptr<Sections> sections;
      std::unique_ptr<Shortcuts> shortcuts;
      std::unique_ptr<Requirements> requirements;
    };
    
    bool parse(::requirements::IStorage &storage, ParserResult &result);
  }
}