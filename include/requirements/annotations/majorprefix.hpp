#pragma once

#include <map>
#include <string>

namespace requirements {
  namespace annotations {
    class MajorPrefix {
    private:
      std::map<std::string, int> majorPrefixStates;
      std::string majorPrefix;
      int majorNumber = 0;
    public:
      void set(const std::string& prefix);
      std::string consume();
    };
  }
}