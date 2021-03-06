#pragma once

#include <vector>
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

namespace test {
  std::vector<::requirements::NodePtr> extractNodes(::requirements::NodeCollection& collection);
}
