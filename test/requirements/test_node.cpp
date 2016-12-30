#include <gtest/gtest.h>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "test/folder.hpp"
#include "test/nodeset.hpp"

static const char* testContent1 = "Some random content\nNo spefic stuff";
static const char* testContent2 = "Some other content";

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
    ASSERT_EQ(node->getAnnotations(), "");
  }
}

TEST(Node, SetParent) {
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
    childNode->setParent(parentNode);
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
    ASSERT_EQ(parentNode->getAnnotations(), "");
    auto children = parentNode->getChildren();
    ASSERT_EQ(children.size(), 1);
    auto childNode = children.front();
    ASSERT_EQ(childNode->getId(), childId);
    ASSERT_EQ(childNode->getContent(), testContent2);
    ASSERT_EQ(childNode->getAnnotations(), "");
    ASSERT_EQ(childNode->getParent(), parentNode);
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