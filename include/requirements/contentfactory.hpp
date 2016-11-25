#pragma once

#include <memory>
#include <string>

namespace requirements {
  
  class IContent;

  std::unique_ptr<IContent> contentFactory(const std::string& typeId);

}