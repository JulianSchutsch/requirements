#include "util/path.hpp"

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
}