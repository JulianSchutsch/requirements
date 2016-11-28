#pragma once

#include <string>

namespace util {
  std::string ensureTrailingSlash(const std::string& path);
  std::string getConfigPath();
}