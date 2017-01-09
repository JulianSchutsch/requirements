#include "annotations/requirements.hpp"

#include "annotations/exception.hpp"

namespace annotations {
  
  Requirement::Requirement(const std::string &a_requirementKey, const std::string &a_text)
    : requirementKey(a_requirementKey)
    , text(a_text) {}
  
  RequirementsBuilder::RequirementsBuilder(Requirements &a_requirements)
    : requirements(a_requirements) {}
  
  void RequirementsBuilder::setMajorPrefix(const std::string& prefix) {
    if(currentScope!=nullptr) {
      throw Exception("Cannot change major prefix with current active scope");
    }
    majorPrefixStates[majorPrefix] = majorNumber;
    majorPrefix = prefix;
    {
      auto continuationIt = majorPrefixStates.find(prefix);
      majorNumber = (continuationIt != majorPrefixStates.end())?continuationIt->second:0;
    }
  }
  
  RequirementsBuilderScope::RequirementsBuilderScope(
    RequirementsBuilder &a_builder,
    ::requirements::Id nodeId,
    const std::string &text)
    : builder(a_builder) {
    if(builder.majorPrefix.empty()) {
      throw Exception("Require major prefix");
    }
    previousScope = builder.currentScope;
    builder.currentScope = this;
    if(previousScope!=nullptr) {
      ++previousScope->number;
      prefix = previousScope->prefix+"."+std::to_string(previousScope->number);
    } else {
      ++builder.majorNumber;
      prefix =  builder.majorPrefix+std::to_string(builder.majorNumber);
    }
    builder.requirements.requirements.emplace(nodeId, Requirement(prefix, text));
  }
  
  RequirementsBuilderScope::~RequirementsBuilderScope() {
    builder.currentScope = previousScope;
  }
  
}