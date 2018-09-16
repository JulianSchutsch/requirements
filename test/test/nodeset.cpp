#include "test/nodeset.hpp"

namespace test {
  std::vector<::requirements::NodePtr> extractNodes(::requirements::NodeCollection& collection) {
    std::vector<::requirements::NodePtr> result;
    for(auto& node: collection) {
      result.emplace_back(node);
    }
    return result;
  }
}