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
    ChildList children{};
    ptrdiff_t referenceCount{0};
    NodeCollection& collection;
    std::string content;
    ChildList::iterator findChild(NodePtr node);
    NodePtr clone(NodeCollection& a_collection);
    friend void intrusive_ptr_add_ref(Node* node);
    friend void intrusive_ptr_release(Node* node);
    friend class NodeCollection;
    
    void clearFromParent();
    bool compareChildren(const Node& other) const;
  public:
    bool operator == (const Node& other) const;
    bool operator != (const Node& other) const { return !this->operator==(other); }
    // Counts the number of siblings before this one.
    int childIndex() const;
    NodePtr getChildNr(int index);
    bool hasParent(const NodePtr& node) const;
    const std::string& getContent() const { return content; }
    void updateContent(const std::string& newContent) { content = newContent; }
    inline NodePtr getParent() const noexcept { return parent; }
    const ChildList getChildren() const noexcept { return children; }
    const Id getId() const noexcept { return id; }
    void setFirstOf(NodePtr node);
    void setLastOf(NodePtr node);
    void setNextTo(NodePtr node);
    void setPreviousTo(NodePtr node);
    explicit Node(NodeCollection& a_collection, Id a_id, std::string&& content);
    void up();
    void down();
    void levelUp();
    void levelDown();
    Node& operator = (const Node&) = delete;
    Node(const Node&) = delete;
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