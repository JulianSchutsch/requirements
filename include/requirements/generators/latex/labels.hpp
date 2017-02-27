#pragma once

#include <ostream>
#include "requirements/nodecollection.hpp"
#include "requirements/annotations/shortcuts.hpp"

namespace requirements{
  namespace generators {
    namespace latex {
      void printLabels(NodeCollection& nodeCollection, annotations::Shortcuts& shortcuts, std::ostream &output);
    }
  }
}