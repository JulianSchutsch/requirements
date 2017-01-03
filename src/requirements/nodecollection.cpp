#include "requirements/nodecollection.hpp"

namespace requirements {

  void NodeCollection::deleteNode(NodePtr node) {
    auto id = node->getId();
    nodes.erase(id);
    node->setParent(nullptr);
    auto children = node->getChildren();
    for(auto& child:children) {
      deleteNode(child);
    }
  }

  void NodeCollection::clear() {
    nodes.clear();
    rootNode = nullptr;
  }
  
  bool NodeCollection::findById(Id id, NodePtr& node) {
    for(auto element: *this) {
      if(element->getId()==id) {
        node = element;
        return true;
      }
    }
    return false;
  }
  
  NodePtr NodeCollection::getNodeById(Id id) {
    auto it = nodes.find(id);
    if(it==nodes.end()) {
      return nullptr;
    }
    return it->second;
  }
  
  NodeCollection::NodeCollection()
    : rootNode(new Node(*this, generateRandomId(), "", "")) {
  }
  
  NodePtr NodeCollection::createNode(Id id, std::string&& content, std::string&& annotations) {
    NodePtr newNode(new Node(*this, id, std::move(content), std::move(annotations)));
    if(id!=newNode->getId())
    {
      throw 1;
    }
    nodes.emplace(id, newNode);
    newNode->setParent(rootNode);
    return newNode;
  }
  
  NodePtr NodeCollection::createNode(const std::string& content) {
    std::string cp = content;
    std::string an = "";
    auto id = generateRandomId();
    return createNode(id, std::move(cp), std::move(an));
  }

}