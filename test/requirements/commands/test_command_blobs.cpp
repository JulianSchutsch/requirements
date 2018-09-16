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

using BlobListResult = std::vector<std::vector<std::string>>;

static BlobListResult listBlobs(::test::BatchThread& b) {
  BlobListResult listResult;
  b.msg_content = [&listResult](const std::string& msg, const std::vector<std::string>& parameters) {
    (void)msg;
    ASSERT_EQ(parameters.size(), 2);
    listResult.emplace_back(parameters);
  };
  b.batch->enqueue(commands::assembleFromString("listblobs"));
  b.wait();
  return listResult;
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
    auto blobFile = b.folder.getName()+"/blob/"+result[0];
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
    auto blobFile = b.folder.getName()+"/blob/"+result[0];
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
  auto defaultLine = listResult[0][1];
  ASSERT_EQ(listResult.size(), 1);
  auto result = listResult[0][0];
  auto it = result.find(".");
  ASSERT_NE(it, std::string::npos);
  auto id = std::string(result, 0, it);
  b.batch->enqueue(std::make_unique<commands::BlobAliases>(id, blobAlias));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  ASSERT_EQ(std::regex_search(listResult[0][1], std::regex(blobAlias)), true);
  b.batch->enqueue(std::make_unique<commands::BlobAliases>(id, ""));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  ASSERT_EQ(boost::algorithm::trim_copy(listResult[0][1]), defaultLine);
  b.batch->enqueue(commands::assembleFromString("blobaliases "+id+" "+blobAlias2));
  b.wait();
  listResult = listBlobs(b);
  ASSERT_EQ(listResult.size(), 1);
  ASSERT_EQ(std::regex_search(listResult[0][1], std::regex(blobAlias2)), true);
  boost::filesystem::remove(fileName);
}
