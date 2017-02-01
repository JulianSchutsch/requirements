#pragma once

#include <map>

#include "requirements/node.hpp"
#include "requirements/id.hpp"

namespace requirements {

  class NodeCollection final {
  private:
    
    using Nodes = std::map<Id, NodePtr>;
    Nodes nodes;
    NodePtr rootNode; // Root-Node always exists
    friend class Node;
  public:
    class Iterator {
    private:
      Nodes::iterator it;
    public:
      NodePtr& operator * () { return (it->second); }
      NodePtr* operator -> () { return &(it->second); }
      bool operator == (const Iterator& other) { return it==other.it; }
      bool operator != (const Iterator& other) { return it!=other.it; }
      Iterator& operator ++ () { ++it; return *this; }
      Iterator(Nodes::iterator a_it) : it(a_it) {}
    };

    bool operator == (const NodeCollection& other) const ;
    bool operator != (const NodeCollection& other) const { return !this->operator==(other);}
    Iterator begin() { return Iterator(nodes.begin()); }
    Iterator end() { return Iterator(nodes.end()); }
    NodePtr createNode(Id id, std::string&& content);
    NodePtr createNode(const std::string& content);
    void deleteNode(NodePtr node);
    inline NodePtr getRootNode() const noexcept { return rootNode; }
    NodePtr getNodeById(Id id);
    void clear();
    NodeCollection();
    NodeCollection(const NodeCollection&) = delete;
    NodeCollection& operator = (const NodeCollection&) = delete;
    std::unique_ptr<NodeCollection> clone();
  };
  
}