#pragma once

#include <list>
#include <memory>

#include "requirements/id.hpp"

namespace annotations {
  class Section {
  private:
    std::list<::requirements::Id> element;
    int depth;
    std::string title;
    std::string description;
    friend class SectionsBuilder;
  public:
    Section(int a_depth, const std::string& a_title, const std::string& description);
    const std::list<::requirements::Id>& getElements() const { return element; };
    int getDepth() const { return depth; }
    const std::string& getTitle() const { return title; }
  };
  
  class Sections {
  private:
    std::list<std::unique_ptr<Section>> sections;
    friend class SectionsBuilder;
  public:
    const std::list<std::unique_ptr<Section>>& getSections() const { return sections; }
  };
  
  class SectionsBuilder {
  private:
    Sections& sections;
    int currentDepth = 0;
    bool hasElements = false;
  public:
    SectionsBuilder(Sections& a_sections);
    void enterSection(const std::string& section, const std::string& description);
    void addElement(::requirements::Id id);
    void leaveSection();
  };
}