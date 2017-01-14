#include <gtest/gtest.h>

#include "test/folder.hpp"
#include "test/batchthread.hpp"

#include "commands/command_new.hpp"

TEST(Commands, CommandNew_ExplizitId) {
  test::BatchThread b;
  auto id = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id));
  auto response = b.wait();
  auto root = response.nodeCollection->getRootNode();
  auto children = root->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id);
  ASSERT_EQ(children.front()->getChildren().size(), 0);
}