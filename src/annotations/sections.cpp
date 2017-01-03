#include "annotations/sections.hpp"

#include "annotations/exception.hpp"

namespace annotations {
  
  Section::Section(int a_depth, const std::string &a_title)
    : depth(a_depth)
    , title(a_title) {}

  SectionsBuilder::SectionsBuilder(Sections& a_sections)
    : sections(a_sections) {}
  
  void SectionsBuilder::enterSection(const std::string& title) {
    if(hasElements) {
      throw Exception("Cannot add sections to a section with common elements");
    }
    ++currentDepth;
    sections.sections.emplace_back(std::make_unique<Section>(currentDepth, title));
  }
  
  void SectionsBuilder::leaveSection() {
    hasElements = false;
    --currentDepth;
  }

}