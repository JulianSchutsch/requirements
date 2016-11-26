#pragma once

namespace requirements {
  
  class IContentVisitor;
  
  class IContent {
  public:
    virtual bool isModified() = 0;
    virtual void visit(IContentVisitor& visitor) = 0;
    virtual void serialize(std::ostream& dest) = 0;
    virtual void deserialize(std::istream& source) = 0;
  };
}