#include "annotations/sections.hpp"

#include "annotations/exception.hpp"

namespace annotations {
  
  Section::Section(const Section&inherited, Elements&& elements, std::unique_ptr<Section>&& newFirstChild)
    : elements(std::move(elements))
    , depth(inherited.depth)
    , title(inherited.title)
    , description(inherited.description)
    , firstChild(std::move(newFirstChild)) {}
  
  Section::Section(const std::string& a_title, const std::string& a_description, Section* a_parent)
    : depth(a_parent?a_parent->depth+1:0)
    , title(a_title)
    , description(a_description)
    , parent(a_parent) {}
  
  SectionsBuilderScope::SectionsBuilderScope(SectionsBuilder& a_builder, const std::string& title, const std::string& description)
    : builder(a_builder) {
    previousScope = builder.currentScope;
    builder.currentScope = this;
    if(previousScope) {
      if(previousScope->hasElements) {
        throw Exception("Cannot add sections to a section with common elements");
      }
    }
    
    auto newSection = std::make_unique<Section>(title, description, (previousScope!=nullptr)?previousScope->section:nullptr);
    section = newSection.get();
    
    if(!previousScope) {
      if(builder.latestTopLevelSection!=nullptr) {
        builder.latestTopLevelSection->nextSibling = std::move(newSection);
      } else {
        builder.sections.firstSection = std::move(newSection);
      }
      builder.latestTopLevelSection = section;
      return;
    }
    if(previousScope->latestChild!=nullptr) {
      previousScope->latestChild->nextSibling = std::move(newSection);
    } else {
      previousScope->section->firstChild = std::move(newSection);
    }
    previousScope->latestChild = section;
  }
  
  SectionsBuilderScope::~SectionsBuilderScope() {
    builder.currentScope = previousScope;
  }
  
  void SectionsBuilderScope::addElement(::requirements::Id id) {
    if(latestChild!=nullptr) {
      throw Exception("Cannot add elements to a section with sub sections");
    }
    hasElements = true;
    section->elements.emplace_back(id);
  }
  
  void SectionsBuilder::addElement(::requirements::Id id) {
    if(currentScope==nullptr) {
      throw Exception("Cannot add element without active SectionsBuilderScope");
    }
    currentScope->addElement(id);
  }
  
  std::unique_ptr<Section> Section::filter(std::function<bool(::requirements::Id)> filterFunction) {
    // Is this element worthy of copying at all?
    // Check the elements
    std::list<::requirements::Id> newElements;
    for(auto& element: elements) {
      if(filterFunction(element)) {
        newElements.emplace_back(element);
      }
    }
    std::unique_ptr<Section> newFirstChild;
    Section* previousSibling = nullptr;
    auto it = firstChild.get();
    while(it!=nullptr) {
      auto newSection = it->filter(filterFunction);
      if(newSection) {
        auto previous = previousSibling;
        previousSibling = newSection.get();
        if(previous!=nullptr) {
          previous->nextSibling = std::move(newSection);
        } else {
          newFirstChild = std::move(newSection);
        }
      }
      it = it->nextSibling.get();
    }
    if(!newFirstChild && newElements.empty()) {
      return nullptr;
    }
    it = newFirstChild.get();
    auto result = std::make_unique<Section>(*this, std::move(newElements), std::move(newFirstChild));
    while(it!=nullptr) {
      it->parent = result.get();
      it = it->nextSibling.get();
    }
    return std::move(result);
  }
  
  Sections Sections::filter(std::function<bool(::requirements::Id)> filterFunction) {
    Sections result;
    auto it = firstSection.get();
    Section* previousSection = nullptr;
    while(it!=nullptr) {
      auto newChildSection = it->filter(filterFunction);
      if(newChildSection) {
        auto previous = previousSection;
        previousSection = newChildSection.get();
        if(previous!=nullptr) {
          previous->nextSibling = std::move(newChildSection);
        } else {
          result.firstSection = std::move(newChildSection);
        }
      }
      it = it->nextSibling.get();
    }
    it = firstSection.get();
    return std::move(result);
  }
  
}