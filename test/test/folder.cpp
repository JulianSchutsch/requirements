#include "test/folder.hpp"

#include <boost/filesystem.hpp>

namespace test {
  UniqueFolder::UniqueFolder() {
    name = boost::filesystem::unique_path().native();
    boost::filesystem::remove_all(name);
  }

  UniqueFolder::~UniqueFolder() {
    boost::filesystem::remove_all(name);
  }
}