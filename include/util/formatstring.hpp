#pragma once

#include <string>
#include <vector>

namespace util {
  
  bool formatString(const std::string& format, const std::vector<std::string>& parameters, std::string& result);
  
}