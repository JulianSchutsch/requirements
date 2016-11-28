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
    : rootNode(new Node(*this, generateRandomId(), "")) {
  }
  
  NodePtr NodeCollection::createNode(Id id, std::string&& content) {
    NodePtr newNode(new Node(*this, id, std::move(content)));
    nodes[id] = newNode.get();
    newNode->setParent(rootNode);
    return newNode;
  }
  
  NodePtr NodeCollection::createNode(const std::string& content) {
    std::string cp = content;
    return createNode(generateRandomId(), std::move(cp));
  }

}