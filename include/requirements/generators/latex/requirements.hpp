#pragma once

#include "requirements/annotations/sections.hpp"
#include "requirements/annotations/requirements.hpp"
#include "requirements/generators/iphasefiles.hpp"

namespace requirements{
  namespace generators {
    namespace latex {
      void printRequirements(const annotations::Sections &sections,
                             const annotations::Requirements &requirements,
                             IPhaseFiles& files);
    }
  }
}