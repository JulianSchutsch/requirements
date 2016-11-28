#include "util/path.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

namespace util {
  std::string ensureTrailingSlash(const std::string& path) {
    if(path.empty()) {
      return "";
    }
    if(path[path.size()-1]!='/') {
      return path+"/";
    }
    return path;
  }
  
  std::string getConfigPath() {
    const char* homedir = getenv("HOME");
    if(homedir==nullptr) {
      homedir = getpwuid(getuid())->pw_dir;
    }
    return ensureTrailingSlash(homedir);
  }
}