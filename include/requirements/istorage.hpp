#pragma once

#include "requirements/id.hpp"

namespace requirements {
  
  class IStorage {
  public:
    virtual Id createBlob() = 0;
    virtual std::string openBlob(Id id) = 0;
    virtual void closeBlob(Id id) = 0;
  };
  
}