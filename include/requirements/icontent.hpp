#pragma once

namespace requirements {
  
  class IContentVisitor;
  
  class IContent {
  public:
    virtual bool isModified() = 0;
    virtual void visit(IContentVisitor& visitor) = 0;
    virtual void serialize(std::ostream& dest) = 0;
    virtual void deserialize(std::istream& source) = 0;
    virtual std::string getDescription() const noexcept = 0;
    
    inline void visit(IContentVisitor&& visitor) { visit(visitor); }
  };
}