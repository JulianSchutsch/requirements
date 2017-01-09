#pragma once

#include <list>
#include <memory>

#include "requirements/id.hpp"

namespace annotations {
  
  class Section final {
  private:
    using Elements = std::list<::requirements::Id>;
    Elements elements;

    int depth = 0;
    std::string title;
    std::string description;
  
    Section* parent = nullptr;
    std::unique_ptr<Section> firstChild;
    std::unique_ptr<Section> nextSibling;

    friend class SectionsBuilderScope;
    friend class Sections;
  public:
    Section(const Section&) = delete;
    Section& operator = (const Section&) = delete;
    Section(const Section& inherited, Elements&& elements, std::unique_ptr<Section>&& newFirstChild);
    Section(const std::string& a_title, const std::string& description, Section* parent);
    int getDepth() const { return depth; }
    const std::string& getTitle() const { return title; }
    const std::string& getDescription() const { return description; }
    const Elements& getElements() const { return elements; }
    std::unique_ptr<Section> filter(std::function<bool(::requirements::Id)>);
  };
  
  class Sections {
  private:
    std::unique_ptr<Section> firstSection;
    friend class SectionsBuilderScope;
  public:
    class Iterator {
    private:
      Section* iterator;
    public:
      Iterator(Section* a_iterator)
        : iterator(a_iterator) {}
      Iterator& operator ++() {
        if(iterator->firstChild) {
          iterator = iterator->firstChild.get();
          return *this;
        }
        if(iterator->nextSibling) {
          iterator = iterator->nextSibling.get();
          return *this;
        }
        if(iterator->parent) {
          if(iterator->parent->nextSibling) {
            iterator = iterator->parent->nextSibling.get();
            return *this;
          }
        }
        iterator = nullptr;
        return *this;
      }
      bool operator == (const Iterator& other) const { return iterator == other.iterator; }
      bool operator != (const Iterator& other) const { return iterator != other.iterator; }
      const Section* operator -> () const { return iterator; }
      const Section& operator * () const { return *iterator; }
    };
    Iterator begin() { return Iterator(firstSection.get()); }
    Iterator end() { return Iterator(nullptr); }
    Sections filter(std::function<bool(::requirements::Id)>);
  };
  
  class SectionsBuilderScope;
  
  class SectionsBuilder final {
  private:
    Sections& sections;
    Section* latestTopLevelSection = nullptr;
    SectionsBuilderScope* currentScope = nullptr;
    friend class SectionsBuilderScope;
  public:
    SectionsBuilder(Sections& a_sections)
      : sections(a_sections) {}
    void addElement(::requirements::Id);
  };
  
  class SectionsBuilderScope final {
  private:
    bool hasElements = false;
    Section* section;
    Section* latestChild = nullptr;
    SectionsBuilderScope* previousScope;
    SectionsBuilder& builder;
  public:
    void addElement(::requirements::Id id);
    SectionsBuilderScope(SectionsBuilder& a_builder, const std::string& title, const std::string& description);
    ~SectionsBuilderScope();
  };
  
}