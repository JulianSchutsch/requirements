#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "requirements/commands/new.hpp"
#include "requirements/commands/delete.hpp"
#include "requirements/commands/command.hpp"

using namespace requirements;

TEST(Commands, Command_Delete) {
  test::BatchThread b;
  auto id = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<commands::New>(id));
  b.batch->enqueue(std::make_unique<commands::Delete>(id));
  auto response = b.wait();
  auto root = response.nodeCollection->getRootNode();
  auto children = root->getChildren();
  ASSERT_EQ(children.size(), 0);
}

TEST(Commands, Command_Delete_Console) {
  test::BatchThread b;
  auto id = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<commands::New>(id));
  b.batch->enqueue(commands::assembleFromString("delete "+::requirements::id_to_string(id)));
  auto response = b.wait();
  auto root = response.nodeCollection->getRootNode();
  auto children = root->getChildren();
  ASSERT_EQ(children.size(), 0);
}