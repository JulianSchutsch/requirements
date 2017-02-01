#include <gtest/gtest.h>

#include <iostream>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "test/folder.hpp"
#include "test/nodeset.hpp"

static const char* testContent1 = "Some random content\nNo spefic stuff";
static const char* testContent2 = "Some other content";
static const char* testContent3 = "Even more content";

TEST(Node, Create) {
  test::UniqueFolder folder;
  ::requirements::Id nodeId;
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto node = collection.createNode(testContent1);
    nodeId = node->getId();
  }
  {
    ::requirements::storage::Text storage(folder.getName(), false);
    auto& collection = storage.getNodeCollection();
    auto nodes = test::extractNodes(collection);
    ASSERT_EQ(nodes.size(), 1);
    auto node = nodes[0];
    ASSERT_EQ(node->getId(), nodeId);
    ASSERT_EQ(node->getParent(), collection.getRootNode());
    ASSERT_EQ(node->getContent(), testContent1);
  }
}

TEST(Node, LastOf) {
  test::UniqueFolder folder;
  ::requirements::Id parentId;
  ::requirements::Id childId;
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto parentNode = collection.createNode(testContent1);
    auto childNode = collection.createNode(testContent2);
    parentId = parentNode->getId();
    childId = childNode->getId();
    childNode->setLastOf(parentNode);
  }
  {
    ::requirements::storage::Text storage(folder.getName(), false);
    auto& collection = storage.getNodeCollection();
    auto rootNode = collection.getRootNode();
    auto topLevel = rootNode->getChildren();
    ASSERT_EQ(topLevel.size(), 1);
    auto parentNode = topLevel.front();
    ASSERT_EQ(parentNode->getId(), parentId);
    ASSERT_EQ(parentNode->getContent(), testContent1);
    auto children = parentNode->getChildren();
    ASSERT_EQ(children.size(), 1);
    auto childNode = children.front();
    ASSERT_EQ(childNode->getId(), childId);
    ASSERT_EQ(childNode->getContent(), testContent2);
    ASSERT_EQ(childNode->getParent(), parentNode);
  }
}

TEST(Node, FirstOf) {
  test::UniqueFolder folder;
  ::requirements::Id parentId;
  ::requirements::Id childId1;
  ::requirements::Id childId2;
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto parentNode = collection.createNode(testContent1);
    auto childNode1 = collection.createNode(testContent2);
    auto childNode2 = collection.createNode(testContent3);
    parentId = parentNode->getId();
    childId1 = childNode1->getId();
    childId2 = childNode2->getId();
    childNode1->setFirstOf(parentNode);
    childNode2->setFirstOf(parentNode);
  }
  {
    ::requirements::storage::Text storage(folder.getName(), false);
    auto& collection = storage.getNodeCollection();
    auto rootNode = collection.getRootNode();
    auto topLevel = rootNode->getChildren();
    ASSERT_EQ(topLevel.size(), 1);
    auto parentNode = topLevel.front();
    ASSERT_EQ(parentNode->getId(), parentId);
    ASSERT_EQ(parentNode->getContent(), testContent1);
    auto children = parentNode->getChildren();
    ASSERT_EQ(children.size(), 2);
    auto childNode2 = children.front();
    ASSERT_EQ(childNode2->getId(), childId2);
    ASSERT_EQ(childNode2->getContent(), testContent3);
    ASSERT_EQ(childNode2->getParent(), parentNode);
    auto childNode1 = children.back();
    ASSERT_EQ(childNode1->getId(), childId1);
    ASSERT_EQ(childNode1->getContent(), testContent2);
    ASSERT_EQ(childNode1->getParent(), parentNode);
  }
}

TEST(Node, Delete) {
  test::UniqueFolder folder;
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto node = collection.createNode(testContent1);
  }
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto nodes = test::extractNodes(collection);
    ASSERT_EQ(nodes.size(), 1);
    collection.deleteNode(nodes[0]);
  }
  {
    ::requirements::storage::Text storage(folder.getName(), false);
    auto& collection = storage.getNodeCollection();
    auto nodes = test::extractNodes(collection);
    ASSERT_EQ(nodes.size(), 0);
    auto rootNode = collection.getRootNode();
    auto topLevel = rootNode->getChildren();
    ASSERT_EQ(topLevel.size(), 0);
  }
}

TEST(Node, Recursive_Delete) {
  test::UniqueFolder folder;
  ::requirements::Id parentId;
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto parent = collection.createNode(testContent1);
    ASSERT_NE(parent->getId(), collection.getRootNode()->getId());
    ASSERT_EQ(parent->getId(), parent->getId());
    auto child = collection.createNode(testContent2);
    child->setLastOf(parent);
    ASSERT_EQ(child->getParent(), parent);
    parentId = parent->getId();
  }
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto root = collection.getRootNode();
    auto topLevel = root->getChildren();
    ASSERT_EQ(topLevel.size(), 1);
    auto parent = topLevel.front();
    ASSERT_EQ(parent->getId(), parentId);
    collection.deleteNode(parent);
  }
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto nodes = test::extractNodes(collection);
    ASSERT_EQ(nodes.size(), 0);
  }
}

TEST(Node, Reparent) {
  test::UniqueFolder folder;
  ::requirements::Id parent1Id;
  ::requirements::Id parent2Id;
  ::requirements::Id childId;
  {
    ::requirements::storage::Text storage(folder.getName(), true);
    auto& collection = storage.getNodeCollection();
    auto parent1 = collection.createNode(testContent1);
    parent1Id = parent1->getId();
    auto parent2 = collection.createNode(testContent2);
    parent2Id = parent2->getId();
    auto child = collection.createNode(testContent3);
    childId = child->getId();
    child->setLastOf(parent1);
    ASSERT_EQ(child->getParent(), parent1);
    child->setLastOf(parent2);
    ASSERT_EQ(child->getParent(), parent2);
  }
  {
    ::requirements::storage::Text storage(folder.getName(), false);
    auto& collection = storage.getNodeCollection();
    auto root = collection.getRootNode();
    auto topLevel = root->getChildren();
    auto parent1 = topLevel.front();
    auto parent2 = topLevel.back();
    ASSERT_EQ(parent1->getId(), parent1Id);
    ASSERT_EQ(parent2->getId(), parent2Id);
    auto children1 = parent1->getChildren();
    auto children2 = parent2->getChildren();
    ASSERT_EQ(children1.size(), 0);
    ASSERT_EQ(children2.size(), 1);
    auto child = children2.front();
    ASSERT_EQ(child->getId(), childId);
    ASSERT_EQ(child->getParent(), parent2);
  }
}

TEST(Node, Up) {
  ::requirements::NodeCollection collection;
  auto node1 = collection.createNode(testContent1);
  auto node2 = collection.createNode(testContent2);
  auto root = collection.getRootNode();
  node1->up();
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.front(), node1);
    ASSERT_EQ(children.back(), node2);
  }
  node2->up();
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.front(), node2);
    ASSERT_EQ(children.back(), node1);
  }
  node2->down();
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.front(), node1);
    ASSERT_EQ(children.back(), node2);
  }
  node2->down();
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.front(), node1);
    ASSERT_EQ(children.back(), node2);
  }
}

TEST(Node, NextTo) {
  ::requirements::NodeCollection collection;
  auto node1 = collection.createNode(testContent1);
  auto node2 = collection.createNode(testContent2);
  auto node3 = collection.createNode(testContent3);
  auto root = collection.getRootNode();
  node1->setNextTo(node3);
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.size(), 3);
    auto it = children.begin();
    ASSERT_EQ((*it), node2);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node3);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node1);
    ASSERT_EQ((*it)->getParent(), root);
  }
  node1->setNextTo(node2);
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.size(), 3);
    auto it = children.begin();
    ASSERT_EQ((*it), node2);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node1);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node3);
    ASSERT_EQ((*it)->getParent(), root);
  }
}

TEST(Node, PreviousTo) {
  ::requirements::NodeCollection collection;
  auto node1 = collection.createNode(testContent1);
  auto node2 = collection.createNode(testContent2);
  auto node3 = collection.createNode(testContent3);
  auto root = collection.getRootNode();
  node3->setPreviousTo(node1);
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.size(), 3);
    auto it = children.begin();
    ASSERT_EQ((*it), node3);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node1);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node2);
    ASSERT_EQ((*it)->getParent(), root);
  }
  node3->setPreviousTo(node2);
  {
    auto children = root->getChildren();
    ASSERT_EQ(children.size(), 3);
    auto it = children.begin();
    ASSERT_EQ((*it), node1);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node3);
    ASSERT_EQ((*it)->getParent(), root);
    ++it;
    ASSERT_EQ((*it), node2);
    ASSERT_EQ((*it)->getParent(), root);
  }
}

TEST(Node, ChildIndex) {
  ::requirements::NodeCollection collection;
  auto node1 = collection.createNode(testContent1);
  auto node2 = collection.createNode(testContent2);
  auto node3 = collection.createNode(testContent3);
  auto root = collection.getRootNode();
  ASSERT_EQ(root->childIndex(), 0);
  ASSERT_EQ(node1->childIndex(), 0);
  ASSERT_EQ(node2->childIndex(), 1);
  ASSERT_EQ(node3->childIndex(), 2);
}

TEST(Node, Clone) {
  ::requirements::NodeCollection collection;
  auto node = collection.createNode(testContent1);
  auto collection2 = collection.clone();
  auto node2 = collection2->getNodeById(node->getId());
  ASSERT_NE(node, node2);
  ASSERT_EQ(node->getChildren().size(), 0);
  ASSERT_EQ(node2->getId(), node->getId());
  ASSERT_EQ(node2->getContent(), node->getContent());
  auto node2p = collection2->createNode(testContent2);
  ASSERT_EQ(node->getChildren().size(), 0);
  node2p->setLastOf(node2);
  ASSERT_EQ(node2p->getParent(), node2);
  ASSERT_EQ(node2->getChildren().size(), 1);
  ASSERT_EQ(node->getChildren().size(), 0);
  ASSERT_EQ(node2->getChildren().front()->getId(), node2p->getId());
  ASSERT_EQ(node2->getChildren().front()->getContent(), testContent2);
}
