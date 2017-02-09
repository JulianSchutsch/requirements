#pragma once

#pragma once

#include <string>
#include <vector>

#include "requirements/annotations/internal/common.hpp"
#include "requirements/annotations/internal/prefixed.hpp"

namespace requirements {
  namespace annotations {
    class Scene final {
    public:
      enum RiskLevel {
        Low,
        Medium,
        High
      };
      struct Risk {
        RiskLevel level;
        std::string description;
        Risk(RiskLevel a_level, std::string a_description)
          : level(a_level)
          , description(a_description) {}
      };
    private:
      std::string key;
      std::string text;
      std::vector<Risk> risks;
    public:
      const std::string& getKey() const { return key; }
      const std::string& getText() const { return text; }
      const std::vector<Risk>& getRisks() const { return risks; }

      Scene(const std::string& a_key, const std::string& a_text, const std::vector<Risk>& a_risks);
    };

    using Scenes = internal::Common<Scene>;
    using ScenesBuilder = internal::PrefixedBuilder<Scene>;
    using ScenesBuilderScope = internal::PrefixedBuilderScope<Scene>;
  }
}