#pragma once

#include "requirements/node.hpp"

namespace requirements {
  class IStorage;
}

namespace req {
  class Status;

  // Helper method for node selection:
  //  If there are parameters, use those to select nodes,
  //  otherwise use a previous selection from the status (using index)
  std::vector<::requirements::NodePtr> selectNodes(
    Status& status, ::requirements::IStorage& storage,
    int index,
    const std::vector<std::string>& parameters);
}