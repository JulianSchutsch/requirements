#pragma once

#include <memory>
#include <list>

#include <boost/intrusive_ptr.hpp>

#include "requirements/id.hpp"

namespace requirements {
  
  class Node;
  class NodeCollection;

  using NodePtr = boost::intrusive_ptr<Node>;
  
  class Node final {
  private:
    Id id;
    using ChildList = std::list<NodePtr>;
    Node* parent=nullptr;
    ChildList children;
    ptrdiff_t referenceCount{0};
    NodeCollection& collection;
    std::string content;
    std::string annotations;
    friend void intrusive_ptr_add_ref(Node* node);
    friend void intrusive_ptr_release(Node* node);
    ChildList::iterator findChild(NodePtr node);
  public:
    bool hasParent(const NodePtr& node) const;
    const std::string& getContent() const { return content; }
    void updateContent(const std::string& newContent) { content = newContent; }
    const std::string& getAnnotations() const { return annotations; }
    inline NodePtr getParent() const noexcept { return parent; }
    const ChildList getChildren() const noexcept { return children; }
    const Id getId() const noexcept { return id; }
    void setParent(NodePtr node);
    explicit Node(NodeCollection& a_collection, Id a_id, std::string&& content, std::string&& annotations);
    void up();
    void down();
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