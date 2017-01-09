#include <gtest/gtest.h>

#include "annotations/sections.hpp"

TEST(AnnotationsSections, Filtera) {
  ::annotations::Sections sections;
  ::annotations::SectionsBuilder builder(sections);
  
  ::requirements::Id element1 = ::requirements::generateRandomId();
  ::requirements::Id element2 = ::requirements::generateRandomId();
  ::requirements::Id element3 = ::requirements::generateRandomId();
  ::requirements::Id element4 = ::requirements::generateRandomId();
  ::requirements::Id element5 = ::requirements::generateRandomId();
  
  {
    ::annotations::SectionsBuilderScope top(builder, "1", "");
    {
      ::annotations::SectionsBuilderScope sec(builder, "1.1", "");
      sec.addElement(element1);
      sec.addElement(element2);
    }
    {
      ::annotations::SectionsBuilderScope sec(builder, "1.2", "");
      sec.addElement(element3);
    }
  }
  {
    ::annotations::SectionsBuilderScope top(builder, "2", "");
    top.addElement(element4);
    top.addElement(element5);
  }
}