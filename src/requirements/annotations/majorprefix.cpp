#include "requirements/annotations/majorprefix.hpp"

#include "requirements/exception.hpp"

namespace requirements {
  namespace annotations {
    void MajorPrefix::set(const std::string &prefix) {
      majorPrefixStates[majorPrefix] = majorNumber;
      majorPrefix = prefix;
      {
        auto continuationIt = majorPrefixStates.find(prefix);
        majorNumber = (continuationIt != majorPrefixStates.end()) ? continuationIt->second : 0;
      }
    }

    std::string MajorPrefix::consume() {
      if(majorPrefix=="") {
        throw Exception(Exception::Kind::Internal, "No valid major prefix set");
      }
      ++majorNumber;
      return majorPrefix + std::to_string(majorNumber);
    }
  }
}