#include "requirements/contentfactory.hpp"

#include "requirements/content_text.hpp"

namespace requirements {
  
  std::unique_ptr<IContent> contentFactory(const std::string& typeId) {
    if(typeId=="txt") {
      return std::make_unique<Content_Text>();
    }
  }
  
}