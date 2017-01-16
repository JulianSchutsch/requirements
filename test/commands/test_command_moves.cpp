#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "commands/command_new.hpp"
#include "commands/command_up.hpp"
#include "commands/command_down.hpp"

TEST(Commands, Command_Up) {
  test::BatchThread b;
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id1));
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id2));
  b.batch->enqueue(std::make_unique<::commands::Command_Up>(id2));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children.front()->getId(), id2);
  ASSERT_EQ(children.back()->getId(), id1);
}

TEST(Commands, Command_Down) {
  test::BatchThread b;
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id1));
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id2));
  b.batch->enqueue(std::make_unique<::commands::Command_Down>(id1));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children.front()->getId(), id2);
  ASSERT_EQ(children.back()->getId(), id1);
}