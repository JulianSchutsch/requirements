#pragma once

#include <string>

namespace requirements {

  class IStorage;
  
  std::string importBlob(IStorage& storage, const std::string& sourceFile);

}