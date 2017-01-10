#include "requirements/select.hpp"

#include <regex>

#include "requirements/nodecollection.hpp"

namespace requirements {
  
  bool selectFromIds(NodeCollection& collection, const std::vector<Id>& ids, std::vector<NodePtr>& result) {
    result.clear();
    result.reserve(ids.size());
    for(auto& id: ids) {
      NodePtr node;
      if(!collection.findById(id, node)) {
        result.clear();
        return false;
      }
      result.emplace_back(node);
    }
    return true;
  }
  
  bool isSelected(const NodePtr& node, const std::vector<NodePtr>& selected) {
    for(auto& element:selected) {
      if(node==element) {
        return true;
      }
    }
    return false;
  }
  
  bool hasSelectedParent(const NodePtr& node, const std::vector<NodePtr>& selected) {
    for(auto& element: selected) {
      if(node->hasParent(element)) {
        return true;
      }
    }
    return false;
  }
  
  std::vector<NodePtr> select(NodeCollection& collection, const std::vector<std::string>& parameters, NodePtr defaultValue) {
    if(parameters.size()==0) {
      return {defaultValue};
    }
    return select(collection, parameters);
  }
  
  std::vector<NodePtr> select(NodeCollection& collection, const std::vector<std::string>& parameters) {
    std::vector<NodePtr> result;
    if(parameters.size()==0) {
      return result;
    }
    std::vector<std::regex> regexes;
    regexes.reserve(parameters.size());
    for(auto& parameter: parameters) {
      regexes.emplace_back(parameter);
    }
    for(auto& node: collection) {
      const std::string idStr = "id:"+id_to_string(node->getId());
      const auto& content = node->getContent();
      bool matched = true;
      for(auto& e: regexes) {
        if((!std::regex_search(content, e)) && (!std::regex_search(idStr, e))) {
          matched = false;
          break;
        }
      }
      if(matched) {
        result.emplace_back(node);
      }
    }
    return result;
  }
}