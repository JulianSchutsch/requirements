#pragma once

#include <string>
#include <map>

#include "requirements/id.hpp"

namespace annotations {
  
  class Requirement final {
  private:
    std::string requirementKey;
    std::string text;
  public:
    const std::string& getRequirementKey() const { return requirementKey; }
    const std::string& getText() const { return text; }
    Requirement(const std::string& a_requirementKey, const std::string& a_text);
  };
  
  class Requirements final {
  private:
    std::map<::requirements::Id, Requirement> requirements;
    friend class RequirementsBuilderScope;
  public:
    bool has(::requirements::Id id) const { return requirements.find(id)!=requirements.end(); }
    const Requirement& get(::requirements::Id id) const { return requirements.at(id); }
  };
  
  class RequirementsBuilderScope;
  
  class RequirementsBuilder final {
  private:
    RequirementsBuilderScope* currentScope = nullptr;
    Requirements& requirements;
    std::string majorPrefix;
    int majorNumber = 0;
    friend class RequirementsBuilderScope;
  public:
    RequirementsBuilder(Requirements& a_requirements);
    void setMajorPrefix(const std::string& prefix);
  };
  
  class RequirementsBuilderScope final {
  private:
    int number = 0;
    std::string prefix;
    RequirementsBuilderScope* previousScope;
    RequirementsBuilder& builder;
  public:
    RequirementsBuilderScope(RequirementsBuilder& a_builder, ::requirements::Id nodeId, const std::string& text);
    ~RequirementsBuilderScope();
  };
  
}