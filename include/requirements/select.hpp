#pragma once

#include <vector>
#include <string>

#include "requirements/node.hpp"

namespace requirements {
  
  class NodeCollection;
  
  std::vector<NodePtr> select(NodeCollection& collection, const std::vector<std::string>& parameters);
}