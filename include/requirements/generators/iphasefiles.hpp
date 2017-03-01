#pragma once

#include <ostream>
#include <string>

namespace requirements {
  namespace generators {
    class IPhaseFiles {
    public:
      virtual std::ostream& getOStream(const std::string& phase) = 0;
      virtual ~IPhaseFiles(){}
    };
  }
}