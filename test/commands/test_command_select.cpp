#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "commands/command_select.hpp"
#include "commands/command_new.hpp"
#include "commands/command.hpp"

TEST(Commands, CommandSelect_ExplicitIds) {
  ::test::BatchThread b;
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_Select>(std::vector<::requirements::Id>({id1, id2})));
  auto response = b.wait();
  ASSERT_EQ(response.status->selections[0].size(), 2);
  ASSERT_EQ(response.status->selections[0][0], id1);
  ASSERT_EQ(response.status->selections[0][1], id2);
}

TEST(Commands, CommandSelect_Console) {
  ::test::BatchThread b;
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  auto id3 = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id1));
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id2));
  b.batch->enqueue(::commands::assembleFromString("select " + ::requirements::id_to_string(id1)));
  {
    auto response = b.wait();
    ASSERT_EQ(response.status->selections[0].size(), 1);
    ASSERT_EQ(response.status->selections[0][0], id1);
  }
  b.batch->enqueue(::commands::assembleFromString("select " + ::requirements::id_to_string(id2)));
  {
    auto response = b.wait();
    ASSERT_EQ(response.status->selections[0].size(), 1);
    ASSERT_EQ(response.status->selections[0][0], id2);
  }
  b.batch->enqueue(::commands::assembleFromString("select " + ::requirements::id_to_string(id3)));
  {
    auto response = b.wait();
    ASSERT_EQ(response.status->selections[0].size(), 0);
  }
  // TODO: Select by content
}