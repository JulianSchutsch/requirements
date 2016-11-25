#pragma once

#include <memory>

#include "requirements/id.hpp"
#include "requirements/node.hpp"

namespace requirements {
  
  class Node;
  class IContent;
  
  class IStorage {
  public:
    virtual NodePtr createNode(std::unique_ptr<IContent>&& content) = 0;
    virtual NodePtr getRootNode() noexcept = 0;
    virtual void save() = 0;
  };
  
}