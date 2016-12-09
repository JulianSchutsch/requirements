#pragma once

#include <vector>
#include <memory>

#include "requirements/node.hpp"

namespace requirements {
  
  class IContent;
  
  std::vector<std::unique_ptr<IContent>> parseContent(NodePtr node);
  std::vector<std::unique_ptr<IContent>> parseAnnotations(NodePtr node);
  
}