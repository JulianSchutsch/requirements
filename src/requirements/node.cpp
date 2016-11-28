#include "requirements/node.hpp"

namespace requirements {
  
  void Node::setParent(NodePtr node) {
    if(parent!=nullptr) {
      parent->children.remove_if([this](const NodePtr& item){return item==this;});
    }
    parent = node.get();
    parent->children.emplace_back(NodePtr(this));
  }

  Node::Node(NodeCollection& a_collection, Id a_id, std::string&& a_content)
    : id(a_id)
    , collection(a_collection)
    , content(std::move(a_content)) {}
  
}