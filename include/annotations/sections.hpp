#pragma once

#include <list>
#include <memory>

#include "requirements/id.hpp"

namespace annotations {
  class Section final {
  private:
    using Elements = std::list<::requirements::Id>;
    Elements elements;
    int depth;
    std::string title;
    std::string description;
    friend class SectionsBuilderScope;
    friend class SectionsNonEmptyFilter;
  public:
    Section(int a_depth, const std::string& a_title, const std::string& description);
    int getDepth() const { return depth; }
    const std::string& getTitle() const { return title; }
    const std::string& getDescription() const { return description; }
    const Elements& getElements() const { return elements; }
  };
  
  class Sections final {
  private:
    using SectionsList = std::list<std::unique_ptr<Section>>;
    SectionsList sections;
    friend class SectionsBuilderScope;
    friend class SectionsNonEmptyFilter;
  public:
    class Iterator {
    private:
      SectionsList::const_iterator iterator;
    public:
      Iterator(SectionsList::const_iterator a_iterator)
        : iterator(a_iterator) {}
      Iterator& operator ++() {
        ++iterator;
        return *this;
      }
      bool operator == (const Iterator& other) const { return iterator == other.iterator; }
      bool operator != (const Iterator& other) const { return iterator != other.iterator; }
      const Section* operator -> () const { return iterator->get(); }
      const Section& operator * () const { return *(iterator->get()); }
    };
    Iterator begin() { return Iterator(sections.cbegin()); }
    Iterator end() { return Iterator(sections.cend()); }
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