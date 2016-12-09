#pragma once

#include <string>

#include "requirements/icontent.hpp"

namespace requirements {
  namespace content {
    class Text : public requirements::IContent {
    private:
      std::string text;
    public:
      Text(const std::string& a_text);
      ContentKind kind() const noexcept override { return ContentKind::Text; }
    };
  }
}