#pragma once

#include <string>
#include <vector>

#include "requirements/id.hpp"

namespace requirements {
  
  class IStorage {
  public:
    virtual std::string createBlob(const std::string& suffix) = 0;
    virtual std::string getBlobFilename(const std::string& id) = 0;
    virtual std::vector<std::string> getBlobs() = 0;
    virtual std::string getLatexFolder() = 0;
  };
  
}