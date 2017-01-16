#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "requirements/commands/command_new.hpp"
#include "requirements/commands/command.hpp"

using namespace requirements;

TEST(Commands, CommandNew_ExplizitId) {
  test::BatchThread b;
  auto id = generateRandomId();
  b.batch->enqueue(std::make_unique<commands::Command_New>(id));
  auto response = b.wait();
  auto root = response.nodeCollection->getRootNode();
  auto children = root->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id);
  ASSERT_EQ(children.front()->getChildren().size(), 0);
}

TEST(Commands, CommandNew_Console) {
  test::BatchThread b;
  auto command = commands::assembleFromString("new");
  ASSERT_NE(command, nullptr);
  b.batch->enqueue(std::move(command));
  auto response = b.wait();
  auto root = response.nodeCollection->getRootNode();
  auto children = root->getChildren();
  auto selections = response.status->selections[0];
  ASSERT_EQ(selections.size(), 1);
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getChildren().size(), 0);
  ASSERT_EQ(children.front()->getId(), selections[0]);
}