#include "requirements/node.hpp"

namespace requirements {
  
  bool Node::hasParent(const NodePtr& node) const {
    auto current = parent;
    while(current) {
      if(node==current) {
        return true;
      }
      current = current->parent;
    }
    return false;
  }
  
  void Node::setParent(NodePtr node) {
    if(parent!=nullptr) {
      parent->children.remove_if([this](const NodePtr& item){return item==this;});
    }
    parent = node.get();
    parent->children.emplace_back(NodePtr(this));
  }

  Node::Node(NodeCollection& a_collection, Id a_id, std::string&& a_content, std::string&& a_annotations)
    : id(a_id)
    , collection(a_collection)
    , content(std::move(a_content))
    , annotations(std::move(a_annotations)) {}
  
}