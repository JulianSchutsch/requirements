#include "annotations/sections.hpp"

#include "annotations/exception.hpp"

namespace annotations {
  
  Section::Section(int a_depth, const std::string &a_title, const std::string& a_description)
    : depth(a_depth)
    , title(a_title)
    , description(a_description) {}

  SectionsBuilder::SectionsBuilder(Sections& a_sections)
    : sections(a_sections) {
    sections.sections.clear();
  }
  
  void SectionsBuilder::addElement(::requirements::Id id) {
    hasElements = true;
    sections.sections.back()->elements.emplace_back(id);
  }
  
  void SectionsBuilder::enterSection(const std::string& title, const std::string& description) {
    if(hasElements) {
      throw Exception("Cannot add sections to a section with common elements");
    }
    ++currentDepth;
    sections.sections.emplace_back(std::make_unique<Section>(currentDepth, title, description));
  }
  
  void SectionsBuilder::leaveSection() {
    hasElements = false;
    --currentDepth;
  }

}