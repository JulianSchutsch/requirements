#pragma once

#include <string>

#include "requirements/icontent.hpp"

namespace requirements {
  
  class Content_Text final : public IContent {
  private:
    bool modified = true;
    std::string content;
  public:
    void visit(IContentVisitor& visitor) override;
    void serialize(std::ostream& dest) override;
    void deserialize(std::istream& source) override;
    bool isModified() override;
    std::string getDescription() const noexcept override;
  };
  
}