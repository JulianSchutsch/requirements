#pragma once

#include <ostream>

#include "requirements/annotations/sections.hpp"

namespace requirements {
  namespace generators {
    class IPhaseFiles;
    namespace latex {
      void iterSections(annotations::Sections& sections,
                        IPhaseFiles& files,
                        std::function<void(const annotations::Section&, std::ostream&)> partGenerator);
    }
  }
}