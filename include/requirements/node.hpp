#pragma once

#include <memory>
#include <list>

#include <boost/intrusive_ptr.hpp>

#include "requirements/id.hpp"

namespace requirements {
  
  class IContent;
  
  class Node;

  using NodePtr = boost::intrusive_ptr<Node>;
  
  class Node final {
  private:
    Id id;
    using ChildList = std::list<NodePtr>;
    Node* parent=nullptr;
    ChildList children;
    std::unique_ptr<IContent> content;
    ptrdiff_t referenceCount;
    friend void intrusive_ptr_add_ref(Node* node);
    friend void intrusive_ptr_release(Node* node);
  public:
    const ChildList getChildren() const noexcept { return children; }
    const Id getId() const noexcept { return id; }
    void setParent(NodePtr node);
    explicit Node(Id a_id, std::unique_ptr<IContent>&& a_content);
  };
  
  inline void intrusive_ptr_add_ref(Node* node) {
    ++node->referenceCount;
  }
  
  inline void intrusive_ptr_release(Node* node) {
    --node->referenceCount;
    if(node->referenceCount==0) {
      delete node;
    }
  }
  
}