#pragma once

#include <vector>
#include <memory>

namespace requirements {

  using Sections = std::vector<std::pair<std::string, std::string>>;
  
  Sections parseContent(const std::string& content);
  
}