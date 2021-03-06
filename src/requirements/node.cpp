#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

namespace requirements {

  bool Node::operator == (const Node& other) const {
    if(id!=other.id) {
      return false;
    }
    if(content!=other.content) {
      return false;
    }
    return compareChildren(other);
  }

  bool Node::compareChildren(const Node &other) const {
    if(children.size()!=other.children.size()) {
      return false;
    }
    auto it1= children.begin();
    auto it2 = other.children.begin();
    while(it1!=children.end()) {
      Node& n1 = **it1;
      Node& n2 = **it2;
      if(n1!=n2) {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  NodePtr Node::clone(NodeCollection& a_collection) {
    auto cp_content = content;
    NodePtr n=new Node(a_collection, id, std::move(cp_content));
    a_collection.nodes.emplace(id, n);
    for(auto& child: children) {
      auto nChild = child->clone(a_collection);
      nChild->setLastOf(n);
    }
    return n;
  }

  NodePtr Node::getChildNr(int index) {
    for(auto child:children) {
      if(index==0) {
        return child;
      }
      --index;
    }
    return nullptr;
  }

  int Node::childIndex() const {
    if(!parent) {
      return 0;
    }
    int pos=0;
    for(auto sibling:parent->children) {
      if(sibling==this) {
        return pos;
      }
      ++pos;
    }
    return 0;
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
  
  void Node::clearFromParent() {
    if(parent!=nullptr) {
      parent->children.remove_if([this](const NodePtr& item){return item==this;});
    }
  }

  void Node::levelUp() {
    setNextTo(parent);
  }

  void Node::levelDown() {
    auto it = parent->findChild(this);
    assert(it!=parent->children.end());
    if(it==parent->children.begin()) {
      return;
    }
    --it;
    setLastOf(*it);
  }
  
  void Node::setNextTo(NodePtr node) {
    clearFromParent();
    parent = node->parent;
    auto it = parent->findChild(node);
    assert(it!=parent->children.end());
    ++it;
    parent->children.emplace(it, this);
  }
  
  void Node::setPreviousTo(NodePtr node) {
    clearFromParent();
    parent = node->parent;
    auto it = parent->findChild(node);
    parent->children.emplace(it, this);
  }
  
  void Node::setFirstOf(NodePtr node) {
    clearFromParent();
    parent = node.get();
    if(parent!=nullptr) {
      parent->children.emplace_front(this);
    }
  }
  
  void Node::setLastOf(NodePtr node) {
    clearFromParent();
    parent = node.get();
    if(parent!=nullptr) {
      parent->children.emplace_back(this);
    }
  }

  Node::Node(NodeCollection& a_collection, Id a_id, std::string&& a_content)
    : id(a_id)
    , collection(a_collection)
    , content(std::move(a_content)) {}
  
}