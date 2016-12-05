#include "requirements/select.hpp"

#include <regex>

#include "requirements/nodecollection.hpp"

namespace requirements {
  std::vector<NodePtr> select(NodeCollection& collection, const std::vector<std::string>& parameters) {
    std::vector<NodePtr> result;
    std::vector<std::regex> regexes;
    regexes.reserve(parameters.size());
    for(auto& parameter: parameters) {
      regexes.emplace_back(parameter);
    }
    for(auto& node: collection) {
      const auto& content = node->getContent();
      bool matched = true;
      for(auto e: regexes) {
        if(!std::regex_match(content, e)) {
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