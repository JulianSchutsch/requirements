#include "requirements/node.hpp"

#include "requirements/icontent.hpp"

namespace requirements {
  
  void Node::setParent(NodePtr node) {
    if(parent!=nullptr) {
      parent->children.remove_if([this](const NodePtr& item){return item==this;});
    }
    parent = node.get();
    parent->children.emplace_back(NodePtr(this));
  }
  
  Node::Node(Id a_id, std::unique_ptr<IContent>&& a_content)
    : id(a_id)
    , content(std::move(a_content)) {}
  
}