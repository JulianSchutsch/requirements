#pragma once

#include <string>
#include <set>

#include "requirements/annotations/internal/common.hpp"

namespace requirements {
  namespace annotations {
    using Phases = internal::Common<std::string>;

    class PhasesBuilder {
    private:
      std::set<std::string> used;
      Phases& phases;
    public:
      PhasesBuilder(Phases& a_phases);
      bool addPhase(::requirements::Id id, const std::string& phaseIdentifier);
    };
  }
}