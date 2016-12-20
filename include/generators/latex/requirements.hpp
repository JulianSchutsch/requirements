#pragma once

#include <string>

namespace requirements {
  class NodeCollection;
  class IStorage;
}

namespace generators {
  namespace latex {
    void  printRequirements(::requirements::NodeCollection &collection, ::requirements::IStorage &storage, const std::string &filename);
  }
}