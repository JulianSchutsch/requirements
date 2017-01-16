#include "requirements/node.hpp"

namespace requirements {

  NodePtr Node::clone(NodeCollection& a_collection) {
    auto cp_content = content;
    NodePtr n=new Node(a_collection, id, std::move(cp_content));
    for(auto& child: children) {
      auto nChild = child->clone(a_collection);
      nChild->setLastOf(n);
    }
    return std::move(n);
  }

  Node::ChildList::iterator Node::findChild(NodePtr node) {
    for(auto it=children.begin();it!=children.end();++it) {
      if(node==*it) {
        return it;
      }
    }
    return children.end();
  }

  void Node::up() {
    if(parent==nullptr) {
      return;
    }
    auto it = parent->findChild(this);
    bool isFirstNode = it==parent->children.begin();
    if(isFirstNode) {
      return;
    }
    auto previous = it;
    --previous;
    std::swap(*it, *previous);
  }

  void Node::down() {
    if(parent==nullptr) {
      return;
    }
    auto it = parent->findChild(this);
    auto next = it;
    ++next;
    bool isLastNode = next==parent->children.end();
    if(isLastNode) {
      return;
    }
    std::swap(*it, *next);
  }

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
  
  void Node::setFirstOf(NodePtr node) {
    if(parent!=nullptr) {
      parent->children.remove_if([this](const NodePtr& item){return item==this;});
    }
    parent = node.get();
    if(parent!=nullptr) {
      parent->children.emplace_front(NodePtr(this));
    }
  }
  
  void Node::setLastOf(NodePtr node) {
    if(parent!=nullptr) {
      parent->children.remove_if([this](const NodePtr& item){return item==this;});
    }
    parent = node.get();
    if(parent!=nullptr) {
      parent->children.emplace_back(NodePtr(this));
    }
  }

  Node::Node(NodeCollection& a_collection, Id a_id, std::string&& a_content)
    : id(a_id)
    , collection(a_collection)
    , content(std::move(a_content)) {}
  
}