#pragma once

#include <string>

namespace requirements {

  class NodeCollection;

  namespace storage {

    void load_text(NodeCollection& collection, const std::string& folder);

  }
  
}