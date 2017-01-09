#include <gtest/gtest.h>

#include "annotations/requirements.hpp"
#include "annotations/exception.hpp"

static const char* testContent1 = "test content 1";
static const char* testContent2 = "test content 2";
static const char* testContent3 = "test content 3";

TEST(AnnotationRequirements, NoPrefix) {
  ::annotations::Requirements requirements;
  ::annotations::RequirementsBuilder builder(requirements);
  try {
    ::annotations::RequirementsBuilderScope majorScope(builder, ::requirements::generateRandomId(), testContent1);
    FAIL();
  }
  catch(::annotations::Exception& e) {
    // pass
  }
  catch(...) {
    FAIL();
  }
}

TEST(AnnotationRequirements, SimpleTree) {
  ::annotations::Requirements requirements;
  ::annotations::RequirementsBuilder builder(requirements);
  ::requirements::Id testId1 = ::requirements::generateRandomId();
  ::requirements::Id testId2 = ::requirements::generateRandomId();
  ::requirements::Id testId3 = ::requirements::generateRandomId();
  builder.setMajorPrefix("TAB");
  {
    ::annotations::RequirementsBuilderScope majorScope(builder, testId1, testContent1);
    {
      ::annotations::RequirementsBuilderScope minorScope1(builder, testId2, testContent2);
    }
    {
      ::annotations::RequirementsBuilderScope minorScope2(builder, testId3, testContent3);
    }
  }
  ASSERT_EQ(requirements.has(::requirements::generateRandomId()), false);
  ASSERT_EQ(requirements.has(testId1), true);
  ASSERT_EQ(requirements.has(testId2), true);
  ASSERT_EQ(requirements.has(testId3), true);
  {
    auto& r1 = requirements.get(testId1);
    ASSERT_EQ(r1.getRequirementKey(), "TAB1");
    ASSERT_EQ(r1.getText(), testContent1);
  }
  {
    auto& r2 = requirements.get(testId2);
    ASSERT_EQ(r2.getRequirementKey(), "TAB1.1");
    ASSERT_EQ(r2.getText(), testContent2);
  }
  {
    auto& r3 = requirements.get(testId3);
    ASSERT_EQ(r3.getRequirementKey(), "TAB1.2");
    ASSERT_EQ(r3.getText(), testContent3);
  }
  
}