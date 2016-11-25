#pragma once

#include <memory>
#include <list>

#include "requirements/id.hpp"

namespace requirements {
  
  class Node final {
  private:
    Id id;
    std::weak_ptr<Node> parent;
    std::list<Node> children;
  public:
    const Id getId() const noexcept { return id; }
    explicit Node(Id a_id) : id(a_id) {}
  };
  
}