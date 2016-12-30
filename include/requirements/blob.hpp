#pragma once

#include <string>
#include "requirements/id.hpp"

namespace requirements {

  class IStorage;
  
  std::string importBlob(IStorage& storage, const std::string& sourceFile);
  std::vector<std::string> selectBlobs(IStorage& storage, const std::vector<std::string>& parameters);

}