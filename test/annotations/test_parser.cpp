#include <gtest/gtest.h>

#include "test/folder.hpp"

#include "requirements/storage/text.hpp"

#include "annotations/parser.hpp"

static ::requirements::NodePtr parentCreate(::requirements::NodeCollection& collection, ::requirements::NodePtr parent, const std::string& text) {
  auto node = collection.createNode(text);
  node->setLastOf(parent);
  return node;
}

static void compareSection(::annotations::Sections::Iterator it, const std::string& title, const std::string& text, int depth, ::requirements::Id id, ::annotations::ParserResult& result) {
  ASSERT_NE(it, result.sections->end());
  auto& section = *it;
  ASSERT_EQ(section.getTitle(), title);
  ASSERT_EQ(section.getDescription(), text);
  ASSERT_EQ(section.getDepth(), depth);
  ASSERT_EQ(result.errors->has(id), false);
  ASSERT_EQ(result.shortcuts->has(id), true);
  ASSERT_EQ(result.shortcuts->get(id), title);
}

static void compareRequirement(::requirements::Id id, const std::string& key, const std::string& text, annotations::ParserResult& result) {
  auto& requirements = *result.requirements;
  ASSERT_EQ(requirements.has(id), true);
  auto& requirement = requirements.get(id);
  ASSERT_EQ(requirement.getRequirementKey(), key);
  ASSERT_EQ(requirement.getText(), text);
  ASSERT_EQ(result.errors->has(id), false);
  ASSERT_EQ(result.shortcuts->has(id), true);
  ASSERT_EQ(result.shortcuts->get(id), key);
}

TEST(AnnotationsParser, Example1) {
  ::test::UniqueFolder folder;
  ::requirements::storage::Text storage(folder.getName(), false);
  auto& collection = storage.getNodeCollection();
  static const char* Phase1Text = "section: Phase1\nSome example for a first phase";
  static const char* Phase2Text = "section: Phase2\nSome example for a second phase";
  static const char* FunctionalText = "requirements: FR Functional requirements";
  static const char* NonFunctionalText = "requirements: NFR Non functional requirements";
  static const char* FRequirementText1 = "Some functional requirement";
  static const char* FSubrequirementText = "Some details";
  static const char* FRequirementText2 = "Some other functional requirement";
  static const char* NFRequirementText1 = "Some non functional requirement";
  static const char* NFRequirementText2 = "Some other non functional requirement";
  auto phase1 = collection.createNode(Phase1Text);
  auto phase2 = collection.createNode(Phase2Text);
  auto functional1 = parentCreate(collection, phase1, FunctionalText);
  auto functional2 = parentCreate(collection, phase2, FunctionalText);
  auto nonFunctional1 = parentCreate(collection, phase1, NonFunctionalText);
  auto nonFunctional2 = parentCreate(collection, phase2, NonFunctionalText);
  auto fr11 = parentCreate(collection, functional1, FRequirementText1);
  auto fr11s = parentCreate(collection, fr11, FSubrequirementText);
  auto fr12 = parentCreate(collection, functional1, FRequirementText2);
  auto fr21 = parentCreate(collection, functional2, FRequirementText1);
  auto fr22 = parentCreate(collection, functional2, FRequirementText2);
  auto nfr11 = parentCreate(collection, nonFunctional1, NFRequirementText1);
  auto nfr12 = parentCreate(collection, nonFunctional1, NFRequirementText2);
  auto nfr21 = parentCreate(collection, nonFunctional2, NFRequirementText1);
  auto nfr22 = parentCreate(collection, nonFunctional2, NFRequirementText2);
  ::annotations::ParserResult parseResult;
  ::annotations::parse(storage, parseResult);
  auto& sections = parseResult.sections;
  auto sectionIterator = sections->begin();
  compareSection(sectionIterator, "Phase1", "Some example for a first phase", 0, phase1->getId(), parseResult);
  ++sectionIterator;
  compareSection(sectionIterator, "Functional requirements", "", 1, functional1->getId(), parseResult);
  {
    auto elements = sectionIterator->getElements();
    ASSERT_EQ(elements.size(), 3);
    auto it = elements.begin();
    compareRequirement(*it, "FR1", "Some functional requirement", parseResult);
    ++it;
    compareRequirement(*it, "FR1.1", "Some details", parseResult);
    ++it;
    compareRequirement(*it, "FR2", "Some other functional requirement", parseResult);
  }
  ++sectionIterator;
  compareSection(sectionIterator, "Non functional requirements", "", 1, nonFunctional1->getId(), parseResult);
  {
    auto elements = sectionIterator->getElements();
    ASSERT_EQ(elements.size(), 2);
    compareRequirement(elements.front(), "NFR1", "Some non functional requirement", parseResult);
    compareRequirement(elements.back(), "NFR2", "Some other non functional requirement", parseResult);
  }
  ++sectionIterator;
  compareSection(sectionIterator, "Phase2", "Some example for a second phase", 0, phase2->getId(), parseResult);
  ++sectionIterator;
  compareSection(sectionIterator, "Functional requirements", "", 1, functional2->getId(), parseResult);
  {
    auto elements = sectionIterator->getElements();
    ASSERT_EQ(elements.size(), 2);
    compareRequirement(elements.front(), "FR3", "Some functional requirement", parseResult);
    compareRequirement(elements.back(), "FR4", "Some other functional requirement", parseResult);
  }
  ++sectionIterator;
  compareSection(sectionIterator, "Non functional requirements", "", 1, nonFunctional2->getId(), parseResult);
  {
    auto elements = sectionIterator->getElements();
    compareRequirement(elements.front(), "NFR3", "Some non functional requirement", parseResult);
    compareRequirement(elements.back(), "NFR4", "Some other non functional requirement", parseResult);
  }
  ++sectionIterator;
  ASSERT_EQ(sectionIterator, sections->end());
}