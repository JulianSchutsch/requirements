#pragma once

#include <ostream>
#include "requirements/annotations/sections.hpp"
#include "requirements/annotations/requirements.hpp"

namespace requirements{
  namespace generators {
    namespace latex {
      void printRequirements(annotations::Sections &sections,
                             annotations::Requirements &requirements,
                             std::ostream &output);
    }
  }
}