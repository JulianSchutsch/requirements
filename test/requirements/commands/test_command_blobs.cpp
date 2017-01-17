#include <gtest/gtest.h>

#include "test/batchthread.hpp"
#include "test/folder.hpp"

#include "requirements/commands/newblob.hpp"
#include "requirements/commands/listblobs.hpp"
#include "requirements/commands/command.hpp"

using namespace requirements;

static const char* blobData = "Some data for the blob;";

TEST(Commands, NewBlob) {
  ::test::BatchThread b;
  ::test::UniqueFolder fileFolder;
  std::string fileName = fileFolder.getName() + "_test";
  {
    std::fstream f(fileName, std::fstream::out);
    f << blobData << std::endl;
  }
  b.batch->enqueue(std::make_unique<commands::NewBlob>(fileName));
  b.wait();
  std::vector<std::string> listResult;
  b.msg_content = [&listResult](const std::string& msg) {listResult.emplace_back(msg);};
  b.batch->enqueue(commands::assembleFromString("listblobs"));
  b.wait();
  ASSERT_EQ(listResult.size(), 1);
  b.batch->enqueue(std::make_unique<commands::NewBlob>(fileName));
  b.wait();
  listResult.clear();
  b.batch->enqueue(commands::assembleFromString("listblobs"));
  b.wait();
  ASSERT_EQ(listResult.size(), 2);
}