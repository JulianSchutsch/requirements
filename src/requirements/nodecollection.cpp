#include "requirements/nodecollection.hpp"

namespace requirements {

  bool NodeCollection::operator == (const NodeCollection& other) const {
    return getRootNode()->compareChildren(*other.getRootNode());
  }

  std::unique_ptr<NodeCollection> NodeCollection::clone() {
    std::unique_ptr<NodeCollection> collection(new NodeCollection());
    collection->rootNode = rootNode->clone(*collection);
    if(trashNode) {
      collection->trashNode = collection->getNodeById(trashNode->getId());
    }
    return collection;
  }

  void NodeCollection::deleteNode(NodePtr node) {
    auto id = node->getId();
    nodes.erase(id);
    if(node==trashNode) {
      trashNode = nullptr;
    }
    node->setLastOf(nullptr);
    auto children = node->getChildren();
    for(auto& child:children) {
      deleteNode(child);
    }
  }

  ::requirements::Id NodeCollection::nodeToTrash(NodePtr node) {
    if(node->hasParent(trashNode) || node==trashNode) {
      auto trashId = trashNode->getId();
      deleteNode(node);
      return trashId;
    }
    ensureTrashNode(::requirements::generateRandomId());
    node->setLastOf(trashNode);
    return trashNode->getId();
  }

  void NodeCollection::clear() {
    nodes.clear();
    rootNode = nullptr;
  }

  NodePtr NodeCollection::ensureTrashNode(::requirements::Id trashId) {
    if(trashNode==nullptr) {
      std::string dummy;
      trashNode = createNode(trashId, std::move(dummy));
    }
    return trashNode;
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