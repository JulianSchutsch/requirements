#include "requirements/annotations/sections.hpp"

#include "requirements/exception.hpp"

namespace requirements {
  namespace annotations {

    void Sections::Iterator::simpleNext() {
      if (iterator->firstChild) {
        iterator = iterator->firstChild.get();
        return;
      }
      while (iterator != nullptr && iterator->nextSibling.get() == nullptr) {
        iterator = iterator->parent;
      }
      if (iterator != nullptr) {
        iterator = iterator->nextSibling.get();
        return;
      }
    }

    void Sections::Iterator::skipPhaseSections() {
      while(iterator!=nullptr && iterator->phaseSection==true) {
        simpleNext();
      }
    }

    Sections::Iterator::Iterator(Section *a_iterator)
    : iterator(a_iterator) {
      skipPhaseSections();
    }

    Sections::Iterator& Sections::Iterator::operator++() {
      simpleNext();
      skipPhaseSections();
      return *this;
    }

    Section::Section(const Section &inherited, Elements &&elements, std::unique_ptr<Section> &&newFirstChild)
      : elements(std::move(elements)), depth(inherited.depth), title(inherited.title),
        description(inherited.description), firstChild(std::move(newFirstChild)), phaseIdentifier(inherited.phaseIdentifier), phaseSection(inherited.phaseSection) {}
    
    Section::Section(const std::string &a_title, const std::string &a_description, const std::string& a_phaseIdentifier, bool a_phaseSection, Section *a_parent)
      : title(a_title)
      , description(a_description)
      , parent(a_parent)
      , phaseIdentifier(a_phaseIdentifier)
      , phaseSection(a_phaseSection){
      if(a_phaseSection) {
        depth = a_parent ? a_parent->depth : 0;
      } else {
        depth = a_parent ? a_parent->depth +1 : 0;
      }
    }
    
    SectionsBuilderScope::SectionsBuilderScope(SectionsBuilder &a_builder, const std::string &title,
                                               const std::string &description, const std::string& phaseIdentifier)
      : builder(a_builder) {
      previousScope = builder.currentScope;
      builder.currentScope = this;
      if (previousScope) {
        if (previousScope->hasElements) {
          throw Exception(Exception::Kind::Internal, "Cannot add sections to a section with common elements");
        }
      }

      std::string inheritedPhaseIdentifier = phaseIdentifier;
      if(inheritedPhaseIdentifier.empty()) {
        if(previousScope!=nullptr) {
          inheritedPhaseIdentifier = previousScope->section->getPhaseIdentifier();
        }
      }

      auto newSection = std::make_unique<Section>(title, description, inheritedPhaseIdentifier, !phaseIdentifier.empty(),
                                                  (previousScope != nullptr) ? previousScope->section : nullptr);
      section = newSection.get();
      
      if (!previousScope) {
        if (builder.latestTopLevelSection != nullptr) {
          builder.latestTopLevelSection->nextSibling = std::move(newSection);
        } else {
          builder.sections.firstSection = std::move(newSection);
        }
        builder.latestTopLevelSection = section;
        return;
      }
      if (previousScope->latestChild != nullptr) {
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
      if (latestChild != nullptr) {
        throw Exception(Exception::Kind::Internal, "Cannot add elements to a section with sub sections");
      }
      hasElements = true;
      section->elements.emplace_back(id);
    }
    
    void SectionsBuilder::addElement(::requirements::Id id) {
      if (currentScope == nullptr) {
        throw Exception(Exception::Kind::Internal, "Camannot add element without active SectionsBuilderScope");
      }
      currentScope->addElement(id);
    }
    
    std::unique_ptr<Section> Section::filter(std::function<bool(::requirements::Id)> filterFunction) const {
      std::list<::requirements::Id> newElements;
      for (auto &element: elements) {
        if (filterFunction(element)) {
          newElements.emplace_back(element);
        }
      }
      std::unique_ptr<Section> newFirstChild;
      Section *previousSibling = nullptr;
      auto it = firstChild.get();
      while (it != nullptr) {
        auto newSection = it->filter(filterFunction);
        if (newSection) {
          auto previous = previousSibling;
          previousSibling = newSection.get();
          if (previous != nullptr) {
            previous->nextSibling = std::move(newSection);
          } else {
            newFirstChild = std::move(newSection);
          }
        }
        it = it->nextSibling.get();
      }
      if (!newFirstChild && newElements.empty()) {
        return nullptr;
      }
      it = newFirstChild.get();
      auto result = std::make_unique<Section>(*this, std::move(newElements), std::move(newFirstChild));
      while (it != nullptr) {
        it->parent = result.get();
        it = it->nextSibling.get();
      }
      return std::move(result);
    }
    
    Sections Sections::filter(std::function<bool(::requirements::Id)> filterFunction) const {
      Sections result;
      auto it = firstSection.get();
      Section *previousSection = nullptr;
      while (it != nullptr) {
        auto newChildSection = it->filter(filterFunction);
        if (newChildSection) {
          auto previous = previousSection;
          previousSection = newChildSection.get();
          if (previous != nullptr) {
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
}