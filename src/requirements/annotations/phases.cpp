#include "requirements/annotations/phases.hpp"

namespace requirements {
  namespace annotations {
    PhasesBuilder::PhasesBuilder(Phases& a_phases)
      : phases(a_phases){}
    bool PhasesBuilder::addPhase(::requirements::Id id, const std::string& phaseIdentifier) {
      if(used.find(phaseIdentifier)!=used.end()) {
        return false;
      }
      used.emplace(phaseIdentifier);
      phases.insert(id, phaseIdentifier);
      return true;
    }
  }
}