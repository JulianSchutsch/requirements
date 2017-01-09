#pragma once

#include <list>
#include <memory>

#include "requirements/id.hpp"

namespace annotations {
  class Section final {
  private:
    std::list<::requirements::Id> elements;
    int depth;
    std::string title;
    std::string description;
    friend class SectionsBuilderScope;
    friend class SectionsNonEmptyFilter;
  public:
    Section(int a_depth, const std::string& a_title, const std::string& description);
    int getDepth() const { return depth; }
    const std::string& getTitle() const { return title; }
  };
  
  class Sections final {
  private:
    using SectionsList = std::list<std::unique_ptr<Section>>;
    SectionsList sections;
    friend class SectionsBuilderScope;
    friend class SectionsNonEmptyFilter;
  public:
    const std::list<std::unique_ptr<Section>>& getSections() const { return sections; }
  };
  
  class SectionsNonEmptyFilter final {
  private:
    using FilterFunction = std::function<bool(::requirements::Id)>;
    Sections& sections;
    FilterFunction filterFunction;
  public:
    class Iterator {
    private:
      SectionsNonEmptyFilter& filter;
      Sections::SectionsList::const_iterator iterator;
      bool empty() {
        for(auto element: (*iterator)->elements) {
          if(filter.filterFunction(element)) {
            return false;
          }
        }
        return true;
      }
      void skipEmpty() {
        while(iterator!=filter.sections.sections.end() and empty()) {
          ++iterator;
        }
      }
    public:
      Iterator& operator ++() {
        ++iterator;
        skipEmpty();
        return *this;
      }
      Iterator(SectionsNonEmptyFilter& a_filter, Sections::SectionsList::const_iterator a_iterator)
        : filter(a_filter)
        , iterator(a_iterator){
        skipEmpty();
      }
      bool operator == (const Iterator& other) const { return iterator==other.iterator; }
    };
    
    SectionsNonEmptyFilter(Sections& a_sections, FilterFunction a_filterFunction)
      : sections(a_sections)
      , filterFunction(a_filterFunction) {}
  };
  
  class SectionsBuilderScope;
  
  class SectionsBuilder final {
  private:
    SectionsBuilderScope* currentScope = nullptr;
    Sections& sections;
    friend class SectionsBuilderScope;
  public:
    SectionsBuilder(Sections& a_sections)
      : sections(a_sections) {}
    void addElement(::requirements::Id);
  };
  
  class SectionsBuilderScope final {
  private:
    bool hasElements = false;
    int depth;
    SectionsBuilderScope* previousScope;
    SectionsBuilder& builder;
  public:
    void addElement(::requirements::Id id);
    SectionsBuilderScope(SectionsBuilder& a_builder, const std::string& title, const std::string& description);
    ~SectionsBuilderScope();
  };
  
}