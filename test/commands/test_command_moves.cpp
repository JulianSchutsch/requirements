#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "commands/command_new.hpp"
#include "commands/command_up.hpp"
#include "commands/command_down.hpp"
#include "commands/command_firstof.hpp"
#include "commands/command_lastof.hpp"

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

TEST(Commands, Command_LastOf) {
  test::BatchThread b;
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id1));
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id2));
  b.batch->enqueue(std::make_unique<::commands::Command_LastOf>(id1, id2));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id2);
  auto subChildren = children.front()->getChildren();
  ASSERT_EQ(subChildren.size(), 1);
  ASSERT_EQ(subChildren.front()->getId(), id1);
}

TEST(Commands, Command_FirstOf) {
  test::BatchThread b;
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  auto id3 = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id1));
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id2));
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id3));
  b.batch->enqueue(std::make_unique<::commands::Command_LastOf>(id2, id1));
  b.batch->enqueue(std::make_unique<::commands::Command_FirstOf>(id3, id1));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id1);
  auto subChildren = children.front()->getChildren();
  ASSERT_EQ(subChildren.size(), 2);
  ASSERT_EQ(subChildren.front()->getId(), id3);
  ASSERT_EQ(subChildren.back()->getId(), id2);
}

