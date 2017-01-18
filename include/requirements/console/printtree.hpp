#pragma once

#include <vector>

#include "requirements/node.hpp"

namespace requirements {
  namespace console {
    
    using PrintFunction = std::function<void(const std::string&)>;
  
    void printTree(PrintFunction p, const requirements::NodePtr& node, const std::vector<requirements::NodePtr> selected);

  }
}