#include "requirements/select.hpp"

#include <regex>

#include "requirements/nodecollection.hpp"

namespace requirements {
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
      const auto& annotations = node->getAnnotations();
      bool matched = true;
      for(auto e: regexes) {
        if((!std::regex_search(content, e)) && (!std::regex_search(annotations, e)) && (!std::regex_search(idStr, e))) {
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