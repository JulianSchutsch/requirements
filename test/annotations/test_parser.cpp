#include <gtest/gtest.h>

#include "test/folder.hpp"

#include "requirements/storage/text.hpp"

#include "annotations/parser.hpp"

TEST(AnnotationsParser, Example1) {
  ::test::UniqueFolder folder;
  ::requirements::storage::Text storage(folder.getName(), false);
  auto& collection = storage.getNodeCollection();
  static const char* Phase1Text = "section: Phase1\n Some example for a first phase";
  static const char* Phase2Text = "section: Phase2\n Some example for a second phase";
  static const char* FunctionalText = "requirements: FR\n Functional requirements";
  static const char* NonFunctionalText = "requirements: NFR\n Non functional requirements";
  auto phase1 = collection.createNode(Phase1Text);
  auto phase2 = collection.createNode(Phase2Text);
}