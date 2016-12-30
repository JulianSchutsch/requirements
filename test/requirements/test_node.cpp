#include <gtest/gtest.h>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "test/folder.hpp"
#include "test/nodeset.hpp"

static const char* testContent1 = "Some random content\nNo spefic stuff";

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
    ASSERT_TRUE(nodes.size()==1);
    auto node = nodes[0];
    ASSERT_TRUE(node->getId()==nodeId);
    ASSERT_TRUE(node->getParent()==collection.getRootNode());
  }
}