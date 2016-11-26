#include "requirements/nodecollection.hpp"

namespace requirements {
  
  NodePtr NodeCollection::getNodeById(Id id) {
    auto it = nodes.find(id);
    if(it==nodes.end()) {
      return nullptr;
    }
    return it->second;
  }
  
  NodeCollection::NodeCollection()
    : rootNode(new Node(*this, generateRandomId(), nullptr)) {
  }
  
  NodePtr NodeCollection::createNode(Id id, std::unique_ptr<IContent>&& content) {
    NodePtr newNode(new Node(*this, id, std::move(content)));
    nodes[id] = newNode.get();
    newNode->setParent(rootNode);
    return newNode;
  }

  NodePtr NodeCollection::createNode(std::unique_ptr<IContent>&& content) {
    return createNode(generateRandomId(), std::move(content));
  }

}