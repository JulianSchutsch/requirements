#pragma once

#include "requirements/annotations/scenes.hpp"
#include "requirements/annotations/sections.hpp"

namespace requirements {
  namespace generators {
    class IPhaseFiles;
    namespace latex {
      void printScenes(const annotations::Sections& sections,
                       const annotations::Scenes& scenes,
                       IPhaseFiles& files);
    }
  }
}