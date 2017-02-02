#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "requirements/commands/select.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/command.hpp"

using namespace requirements;

TEST(Commands, CommandSelect_ExplicitIds) {
  ::test::BatchThread b;
  auto id1 = generateRandomId();
  auto id2 = generateRandomId();
  b.batch->enqueue(std::make_unique<commands::Select>(std::vector<::requirements::Id>({id1, id2})));
  auto response = b.wait();
  ASSERT_EQ(response.status->selections[0].size(), 2);
  ASSERT_EQ(response.status->selections[0][0], id1);
  ASSERT_EQ(response.status->selections[0][1], id2);
}

TEST(Commands, CommandSelect_Console) {
  ::test::BatchThread b;
  auto id1 = generateRandomId();
  auto id2 = generateRandomId();
  auto id3 = generateRandomId();
  b.batch->enqueue(std::make_unique<commands::New>(id1));
  b.batch->enqueue(std::make_unique<commands::New>(id2));
  b.batch->enqueue(commands::assembleFromString("select " + ::requirements::id_to_string(id1)));
  {
    auto response = b.wait();
    ASSERT_EQ(response.status->selections[0].size(), 1);
    ASSERT_EQ(response.status->selections[0][0], id1);
  }
  b.batch->enqueue(commands::assembleFromString("select " + ::requirements::id_to_string(id2)));
  {
    auto response = b.wait();
    ASSERT_EQ(response.status->selections[0].size(), 1);
    ASSERT_EQ(response.status->selections[0][0], id2);
  }
  b.batch->enqueue(commands::assembleFromString("select " + ::requirements::id_to_string(id3)));
  {
    auto response = b.wait();
    ASSERT_EQ(response.status->selections[0].size(), 0);
  }
  // TODO: Select by content
}
