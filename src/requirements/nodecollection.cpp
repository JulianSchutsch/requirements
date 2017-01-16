#include "requirements/nodecollection.hpp"

namespace requirements {

  std::unique_ptr<NodeCollection> NodeCollection::clone() {
    std::unique_ptr<NodeCollection> collection(new NodeCollection());
    collection->rootNode = rootNode->clone(*collection);
    return std::move(collection);
  }

  void NodeCollection::deleteNode(NodePtr node) {
    auto id = node->getId();
    nodes.erase(id);
    node->setLastOf(nullptr);
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
    : rootNode(new Node(*this, generateRandomId(), "")) {
  }
  
  NodePtr NodeCollection::createNode(Id id, std::string&& content) {
    NodePtr newNode(new Node(*this, id, std::move(content)));
    if(id!=newNode->getId())
    {
      throw 1;
    }
    nodes.emplace(id, newNode);
    newNode->setLastOf(rootNode);
    return newNode;
  }
  
  NodePtr NodeCollection::createNode(const std::string& content) {
    std::string cp = content;
    auto id = generateRandomId();
    return createNode(id, std::move(cp));
  }

}