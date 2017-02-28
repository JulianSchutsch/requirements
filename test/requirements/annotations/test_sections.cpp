#include <gtest/gtest.h>

#include "requirements/annotations/sections.hpp"

using namespace requirements;

void compareSections(annotations::Sections& sections, const std::vector<std::pair<std::string, int>> data) {
  auto it = sections.begin();
  auto dit = data.begin();
  while(it!=sections.end()) {
    ASSERT_NE(dit, data.end());
    ASSERT_EQ(it->getTitle(), dit->first);
    ASSERT_EQ(it->getDepth(), dit->second);
    ++it;
    ++dit;
  }
  ASSERT_EQ(dit, data.end());
}

TEST(AnnotationsSections, Filter) {
  annotations::Sections sections;
  annotations::SectionsBuilder builder(sections);
  
  Id element1 = generateRandomId();
  Id element2 = generateRandomId();
  Id element3 = generateRandomId();
  Id element4 = generateRandomId();
  Id element5 = generateRandomId();
  Id element6 = generateRandomId();
  
  {
    annotations::SectionsBuilderScope top(builder, "1", "", "");
    {
      annotations::SectionsBuilderScope sec(builder, "1.1", "", "");
      sec.addElement(element1);
      sec.addElement(element2);
    }
    {
      annotations::SectionsBuilderScope sec(builder, "1.2", "", "");
      sec.addElement(element3);
    }
    {
      annotations::SectionsBuilderScope sec(builder, "1.3", "", "");
      {
        annotations::SectionsBuilderScope subsec(builder, "1.3.1", "", "");
        subsec.addElement(element6);
      }
    }
  }
  {
    annotations::SectionsBuilderScope top(builder, "2", "", "");
    top.addElement(element4);
    top.addElement(element5);
  }
  {
    auto filtered = sections.filter([&](::requirements::Id id) {
      return id == element1 || id == element2;
    });
    compareSections(filtered, {{"1",   0}, {"1.1", 1}});
  }
  {
    auto filtered = sections.filter([&](::requirements::Id id) {
      return id == element1;
    });
    compareSections(filtered, {{"1", 0}, {"1.1", 1}});
  }
  {
    auto filtered = sections.filter([&](::requirements::Id id) {
      return id==element2 || id==element4;
    });
    compareSections(filtered, {{"1", 0}, {"1.1", 1}, {"2", 0}});
  }
  {
    auto filtered = sections.filter([&](::requirements::Id id) { (void)id; return false; });
    compareSections(filtered, {});
  }
  {
    auto filtered = sections.filter([&](::requirements::Id id) {
      return id==element6 || id==element5;
    });
    compareSections(filtered, {{"1",0}, {"1.3", 1}, {"1.3.1", 2}, {"2", 0}});
  }
}