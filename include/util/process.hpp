#pragma once

#include <string>
#include <vector>

namespace util {
  bool runProcess(const std::string& command, const std::vector<std::string>& parameters);
}