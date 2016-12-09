#pragma once

#include <vector>
#include <memory>

namespace requirements {

  using Sections = std::vector<std::pair<std::string, std::string>>;
  
  Sections parseSections(const std::string& content);
  
}