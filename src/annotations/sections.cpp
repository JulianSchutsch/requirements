#include "annotations/sections.hpp"

#include "annotations/exception.hpp"

namespace annotations {
  
  Section::Section(int a_depth, const std::string &a_title, const std::string& a_description)
    : depth(a_depth)
    , title(a_title)
    , description(a_description) {}
  
  SectionsBuilderScope::SectionsBuilderScope(SectionsBuilder& a_builder, const std::string& title, const std::string& description)
    : builder(a_builder) {
    previousScope = builder.currentScope;
    builder.currentScope = this;
    if(previousScope) {
      if(previousScope->hasElements) {
        throw Exception("Cannot add sections to a section with common elements");
      }
      depth = previousScope->depth+1;
    }
    builder.sections.sections.emplace_back(std::make_unique<Section>(depth, title, description));
  }
  
  SectionsBuilderScope::~SectionsBuilderScope() {
    builder.currentScope = previousScope;
  }
  
  void SectionsBuilderScope::addElement(::requirements::Id id) {
    hasElements = true;
    builder.sections.sections.back()->elements.emplace_back(id);
  }
  
  void SectionsBuilder::addElement(::requirements::Id id) {
    if(currentScope==nullptr) {
      throw Exception("Cannot add element without active SectionsBuilderScope");
    }
    currentScope->addElement(id);
  }
  
}