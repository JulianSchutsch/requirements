#pragma once

#include <string>

#include "requirements/icontent.hpp"

namespace requirements {
  
  class Content_Text : public IContent {
  private:
    std::string content;
  public:
    void visit(IContentVisitor& visitor) override;
    void serialize(std::ostream& dest) override;
    void deserialize(std::istream& source) override;
  };
  
}