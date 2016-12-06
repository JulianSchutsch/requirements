#pragma once

#include <ostream>

#include "requirements/node.hpp"

namespace req {
  namespace console {
  
    void printTree(std::ostream& stream, const requirements::NodePtr& node, const std::vector<requirements::NodePtr> selected);

  }
}