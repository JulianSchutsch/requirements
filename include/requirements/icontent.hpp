#pragma once

namespace requirements {
  
  enum class ContentKind {
    Text
  };

  class IContent {
  public:
    virtual ContentKind kind() const noexcept = 0;
  };
  
}