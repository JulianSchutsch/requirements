#pragma once

#include <string>
#include <vector>

#include "requirements/annotations/internal/common.hpp"
#include "requirements/annotations/internal/prefixed.hpp"

namespace requirements {
  namespace annotations {
    class Acceptance final {
    private:
      std::string key;
      std::string text;
      std::vector<::requirements::Id> accepts;
    public:
      const std::string& getKey() const { return key; }
      const std::string& getText() const { return text; }
      const std::vector<::requirements::Id>& getAccepts() const { return accepts; }
      
      Acceptance(const std::string& a_key, const std::string& a_text, const std::vector<::requirements::Id>& a_accepts);
    };
    
    using Acceptances = internal::Common<Acceptance>;
    using AcceptancesBuilder = internal::PrefixedBuilder<Acceptance>;
    using AcceptancesBuilderScope = internal::PrefixedBuilderScope<Acceptance>;
  }
}