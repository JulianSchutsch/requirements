#pragma once

namespace requirements {
  
  class Content_Text;

  class IContentVisitor {
  public:
    virtual void handleContent(Content_Text&) = 0;
  };
  
}