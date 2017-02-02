#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "requirements/commands/new.hpp"
#include "requirements/commands/edit.hpp"
#include "requirements/commands/command.hpp"

TEST(Commands, Command_Edit) {
  ::test::BatchThread b;
  ::requirements::Id expectedEditId = ::requirements::generateRandomId();
  ::requirements::Id secondId = ::requirements::generateRandomId();
  volatile bool editReceived = false;
  b.msg_edit = [&expectedEditId, &editReceived](::requirements::NodePtr node){
    ASSERT_EQ(editReceived, false);
    ASSERT_EQ(expectedEditId, node->getId());
    editReceived = true;
  };
  b.batch->enqueue(std::make_unique<::requirements::commands::New>(expectedEditId));
  b.batch->enqueue(::requirements::commands::assembleFromString("edit"));
  b.wait();
  ASSERT_EQ(editReceived, true);
  editReceived = false;
  b.batch->enqueue(std::make_unique<::requirements::commands::New>(secondId));
  b.batch->enqueue(::requirements::commands::assembleFromString("edit "+::requirements::id_to_string(expectedEditId)));
  b.wait();
  ASSERT_EQ(editReceived, true);
  editReceived = false;
  expectedEditId = secondId;
  b.batch->enqueue(::requirements::commands::assembleFromString("edit "+::requirements::id_to_string(expectedEditId)));
  b.wait();
  ASSERT_EQ(editReceived, true);
}