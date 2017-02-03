#pragma once

#include <string>

#include "requirements/annotations/internal/common.hpp"
#include "requirements/annotations/internal/prefixed.hpp"

namespace requirements {
  namespace annotations {
    
    class Requirement final {
    private:
      std::string key;
      std::string text;
      bool treeCoveredByAcceptance = false;
      bool coveredByAcceptance = false;
    public:
      const std::string& getKey() const { return key; }
      
      const std::string& getText() const { return text; }

      void setTreeCoveredByAcceptance(bool value) { treeCoveredByAcceptance = value; }
      void setCoveredByAcceptance(bool value) { coveredByAcceptance = value; }
      bool isCoveredByAcceptance() const { return coveredByAcceptance; }
      bool isTreeCoveredByAcceptance() const { return treeCoveredByAcceptance; }
      
      Requirement(const std::string& a_key, const std::string& a_text);
    };
    
    using Requirements = internal::Common<Requirement>;
    using RequirementsBuilder = internal::PrefixedBuilder<Requirement>;
    using RequirementsBuilderScope = internal::PrefixedBuilderScope<Requirement>;
  }
}