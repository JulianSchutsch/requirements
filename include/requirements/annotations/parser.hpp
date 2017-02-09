#pragma once

#include <map>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"

#include "requirements/annotations/sections.hpp"
#include "requirements/annotations/errors.hpp"
#include "requirements/annotations/shortcuts.hpp"
#include "requirements/annotations/requirements.hpp"
#include "requirements/annotations/acceptances.hpp"
#include "requirements/annotations/scenes.hpp"

namespace requirements {
  namespace annotations {
    
    struct ParserResult {
      std::unique_ptr<Errors> errors;
      std::unique_ptr<Sections> sections;
      std::unique_ptr<Shortcuts> shortcuts;
      std::unique_ptr<Requirements> requirements;
      std::unique_ptr<Acceptances> acceptances;
      std::unique_ptr<Scenes> scenes;
    };
    
    bool parse(::requirements::IStorage &storage, ParserResult &result);
  }
}