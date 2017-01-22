#include <gtest/gtest.h>

#include <fstream>
#include <regex>
#include <boost/algorithm/string.hpp>

#include "test/batchthread.hpp"

#include "util/stringfile.hpp"

#include "requirements/commands/newblob.hpp"
#include "requirements/commands/listblobs.hpp"
#include "requirements/commands/command.hpp"
#include "requirements/commands/blobaliases.hpp"

using namespace requirements;

static const char* blobData = "Some data for the blob;";
static const char* blobAlias = "blobAlias1";
static const char* blobAlias2 = "blobAlias2";

static std::vector<std::string> listBlobs(::test::BatchThread& b) {
  std::vector<std::string> listResult;
  b.msg_content = [&listResult](const std::string& msg) {listResult.emplace_back(msg);};
  b.batch->enqueue(commands::assembleFromString("listblobs"));
  b.wait();
  return std::move(listResult);
}

TEST(Commands, NewBlob) {
  ::test::BatchThread b;
  ::test::UniqueFolder fileFolder;
  std::string fileName = fileFolder.getName() + "_test.txt";
  {
    std::fstream f(fileName, std::fstream::out);
    f << blobData;
  }
  b.batch->enqueue(std::make_unique<commands::NewBlob>(fileName));
  b.wait();
  auto listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  b.batch->enqueue(std::make_unique<commands::NewBlob>(fileName));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 2);
  for(auto result: listResult) {
    auto it = result.find("->");
    ASSERT_NE(it, std::string::npos);
    auto blobFile = b.folder.getName()+"/blob/"+std::string(result, 0, it);
    ASSERT_EQ(util::readFileToString(blobFile), blobData);
  }
  boost::filesystem::remove(fileName);
}

TEST(Commands, NewBlob_Console) {
  ::test::BatchThread b;
  ::test::UniqueFolder fileFolder;
  std::string fileName = fileFolder.getName() + "_test.txt";
  {
    std::fstream f(fileName, std::fstream::out);
    f << blobData;
  }
  b.batch->enqueue(commands::assembleFromString("newblob "+fileName));
  b.wait();
  auto listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  for(auto result: listResult) {
    auto it = result.find("->");
    ASSERT_NE(it, std::string::npos);
    auto blobFile = b.folder.getName()+"/blob/"+std::string(result, 0, it);
    ASSERT_EQ(util::readFileToString(blobFile), blobData);
  }
  boost::filesystem::remove(fileName);
}

TEST(Commands, BlobAlias) {
  ::test::BatchThread b;
  ::test::UniqueFolder fileFolder;
  std::string fileName = fileFolder.getName() + "_test.txt";
  {
    std::fstream f(fileName, std::fstream::out);
    f << blobData;
  }
  b.batch->enqueue(std::make_unique<commands::NewBlob>(fileName));
  b.wait();
  auto listResult = listBlobs(b);
  auto defaultLine = listResult[0];
  ASSERT_EQ(listResult.size(), 1);
  auto result = listResult[0];
  auto it = result.find(".");
  ASSERT_NE(it, std::string::npos);
  auto id = std::string(result, 0, it);
  b.batch->enqueue(std::make_unique<commands::BlobAliases>(id, blobAlias));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  ASSERT_EQ(std::regex_search(listResult[0], std::regex(blobAlias)), true);
  b.batch->enqueue(std::make_unique<commands::BlobAliases>(id, ""));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  ASSERT_EQ(boost::algorithm::trim_copy(listResult[0]), defaultLine);
  b.batch->enqueue(commands::assembleFromString("blobaliases "+id+" "+blobAlias2));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  ASSERT_EQ(std::regex_search(listResult[0], std::regex(blobAlias2)), true);
  boost::filesystem::remove(fileName);
}
