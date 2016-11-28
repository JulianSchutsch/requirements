#pragma once

#include <map>

#include "requirements/node.hpp"
#include "requirements/id.hpp"

namespace requirements {
  
  class IStorage;
  
  class NodeCollection final {
  private:
    using Nodes = std::map<Id, NodePtr>;
    Nodes nodes;
    NodePtr rootNode; // Root-Node always exists
  public:
    class Iterator {
    private:
      Nodes::iterator it;
    public:
      Node& operator * () { return *it->second; }
      Node* operator -> () { return &*it->second; }
      bool operator == (const Iterator& other) { return it==other.it; }
      bool operator != (const Iterator& other) { return it!=other.it; }
      Iterator& operator ++ () { ++it; return *this; }
      Iterator(Nodes::iterator a_it) : it(a_it) {}
    };
    
    Iterator begin() { return Iterator(nodes.begin()); }
    Iterator end() { return Iterator(nodes.end()); }
    NodePtr createNode(Id id, std::string&& content);
    NodePtr createNode(const std::string& content);
    inline NodePtr getRootNode() noexcept { return rootNode; }
    NodePtr getNodeById(Id id);
    NodeCollection();
  };
  
}