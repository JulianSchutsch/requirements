#include <gtest/gtest.h>

#include "test/folder.hpp"
#include "test/batchthread.hpp"

#include "requirements/id.hpp"

#include "commands/command_new.hpp"

TEST(Commands, CommandNew_ExplizitId) {
  test::BatchThread b;
  auto id = ::requirements::generateRandomId();
  b.batch->enqueue(std::make_unique<::commands::Command_New>(id));
  auto response = b.wait();
}