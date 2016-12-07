#pragma once

#include "requirements/id.hpp"

namespace requirements {
  
  class IStorage {
  public:
    virtual std::string createBlob(const std::string& suffix) = 0;
    virtual std::string openBlob(const std::string& id) = 0;
    virtual void closeBlob(const std::string& id) = 0;
  };
  
}