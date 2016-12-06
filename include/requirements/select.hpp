#pragma once

#include <vector>
#include <string>

#include "requirements/node.hpp"
#include "requirements/id.hpp"

namespace requirements {
  
  class NodeCollection;
  
  std::vector<NodePtr> select(NodeCollection& collection, const std::vector<std::string>& parameters);
  std::vector<NodePtr> select(NodeCollection& collection, const std::vector<std::string>& parameters, NodePtr defaultValue);
  bool hasSelectedParent(const NodePtr& node, const std::vector<NodePtr>& selected);
  bool isSelected(const NodePtr& node, const std::vector<NodePtr>& selected);
  bool selectFromIds(NodeCollection& collection, const std::vector<Id>& ids, std::vector<NodePtr>& result);
}