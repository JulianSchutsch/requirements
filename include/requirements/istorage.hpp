#pragma once

#include <memory>

#include "requirements/id.hpp"

namespace requirements {
  
  class Node;
  
  class IStorage {
  public:
    virtual std::shared_ptr<Node> createNode() = 0;
    virtual std::shared_ptr<Node> getRootNode() noexcept = 0;
    virtual void save() = 0;
  };
  
}