#pragma once

#include <ostream>

namespace requirements{
  namespace annotations {
    class Sections;
    class Requirements;
  }
  namespace generators {
    namespace latex {
      void printRequirements(annotations::Sections &sections,
                             annotations::Requirements &requirements,
                             std::ostream &output);
    }
  }
}