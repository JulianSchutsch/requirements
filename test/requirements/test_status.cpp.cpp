#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include "test/folder.hpp"

#include "requirements/status.hpp"

TEST(Status, SaveLoad) {
  ::test::UniqueFolder folder;
  auto configFile = folder.getName()+"_status.xml";
  auto id1 = ::requirements::generateRandomId();
  auto id2 = ::requirements::generateRandomId();
  static const char* FolderTestName = "testFolderName/./x";
  {
    ::requirements::Status status;
    status.selections[0] = {id1, id2};
    status.folder = FolderTestName;
    status.save(configFile);
  }
  {
    ::requirements::Status status;
    status.load(configFile);
    ASSERT_EQ(status.selections[0].size(), 2);
    ASSERT_EQ(status.selections[0].front(), id1);
    ASSERT_EQ(status.selections[0].back(), id2);
    ASSERT_EQ(status.folder, FolderTestName);
  }
  boost::filesystem::remove(configFile);
}