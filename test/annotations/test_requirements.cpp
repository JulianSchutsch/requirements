#include <gtest/gtest.h>

#include "annotations/requirements.hpp"
#include "annotations/exception.hpp"

static const char* testContent1 = "test content 1";
static const char* testContent2 = "test content 2";
static const char* testContent3 = "test content 3";
static const char* testContent4 = "test content 4";

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

TEST(AnnotationRequirements, NestedRequirements) {
  ::annotations::Requirements requirements;
  ::annotations::RequirementsBuilder builder(requirements);
  ::requirements::Id testId1 = ::requirements::generateRandomId();
  ::requirements::Id testId2 = ::requirements::generateRandomId();
  ::requirements::Id testId3 = ::requirements::generateRandomId();
  ::requirements::Id testId4 = ::requirements::generateRandomId();
  ::requirements::Id testId5 = ::requirements::generateRandomId();
  ::requirements::Id testId6 = ::requirements::generateRandomId();
  ::requirements::Id testId7 = ::requirements::generateRandomId();
  ::requirements::Id testId8 = ::requirements::generateRandomId();
  ::requirements::Id testId9 = ::requirements::generateRandomId();
  builder.setMajorPrefix("TAB");
  {
    ::annotations::RequirementsBuilderScope majorScope(builder, testId1, testContent1);
    {
      ::annotations::RequirementsBuilderScope minorScope(builder, testId2, testContent2);
      {
        ::annotations::RequirementsBuilderScope patchScope(builder, testId3, testContent3);
      }
    }
  }
  {
    ::annotations::RequirementsBuilderScope majorScope(builder, testId4, testContent1);
    {
      ::annotations::RequirementsBuilderScope minorScope(builder, testId5, testContent2);
      {
        ::annotations::RequirementsBuilderScope patchScope(builder, testId6, testContent3);
      }
      {
        ::annotations::RequirementsBuilderScope patchScope(builder, testId7, testContent1);
      }
    }
    {
      ::annotations::RequirementsBuilderScope minorScope(builder, testId8, testContent2);
      {
        ::annotations::RequirementsBuilderScope patchScope(builder, testId9, testContent1);
      }
    }
  }
  ASSERT_EQ(requirements.has(::requirements::generateRandomId()), false);
  ASSERT_EQ(requirements.has(testId1), true);
  ASSERT_EQ(requirements.has(testId2), true);
  ASSERT_EQ(requirements.has(testId3), true);
  ASSERT_EQ(requirements.has(testId4), true);
  ASSERT_EQ(requirements.has(testId5), true);
  ASSERT_EQ(requirements.has(testId6), true);
  ASSERT_EQ(requirements.has(testId7), true);
  ASSERT_EQ(requirements.has(testId8), true);
  ASSERT_EQ(requirements.has(testId9), true);
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
    ASSERT_EQ(r3.getRequirementKey(), "TAB1.1.1");
    ASSERT_EQ(r3.getText(), testContent3);
  }
  {
    auto& r4 = requirements.get(testId4);
    ASSERT_EQ(r4.getRequirementKey(), "TAB2");
    ASSERT_EQ(r4.getText(), testContent1);
  }
  {
    auto& r5 = requirements.get(testId5);
    ASSERT_EQ(r5.getRequirementKey(), "TAB2.1");
    ASSERT_EQ(r5.getText(), testContent2);
  }
  {
    auto& r6 = requirements.get(testId6);
    ASSERT_EQ(r6.getRequirementKey(), "TAB2.1.1");
    ASSERT_EQ(r6.getText(), testContent3);
  }
  {
    auto& r7 = requirements.get(testId7);
    ASSERT_EQ(r7.getRequirementKey(), "TAB2.1.2");
    ASSERT_EQ(r7.getText(), testContent1);
  }
  {
    auto& r8 = requirements.get(testId8);
    ASSERT_EQ(r8.getRequirementKey(), "TAB2.2");
    ASSERT_EQ(r8.getText(), testContent2);
  }
  {
    auto& r9 = requirements.get(testId9);
    ASSERT_EQ(r9.getRequirementKey(), "TAB2.2.1");
    ASSERT_EQ(r9.getText(), testContent1);
  }
}

TEST(AnnotationRequirements, Continuation) {
  ::annotations::Requirements requirements;
  ::annotations::RequirementsBuilder builder(requirements);
  ::requirements::Id testId1 = ::requirements::generateRandomId();
  ::requirements::Id testId2 = ::requirements::generateRandomId();
  ::requirements::Id testId3 = ::requirements::generateRandomId();
  ::requirements::Id testId4 = ::requirements::generateRandomId();
  builder.setMajorPrefix("TABA");
  {
    ::annotations::RequirementsBuilderScope scope1(builder, testId1, testContent1);
  }
  builder.setMajorPrefix("TABB");
  {
    ::annotations::RequirementsBuilderScope scope2(builder, testId2, testContent2);
  }
  builder.setMajorPrefix("TABA");
  {
    ::annotations::RequirementsBuilderScope scope3(builder, testId3, testContent3);
  }
  builder.setMajorPrefix("TABB");
  {
    ::annotations::RequirementsBuilderScope scope4(builder, testId4, testContent4);
  }
  ASSERT_EQ(requirements.has(testId1), true);
  ASSERT_EQ(requirements.has(testId2), true);
  ASSERT_EQ(requirements.has(testId3), true);
  ASSERT_EQ(requirements.has(testId4), true);
  {
    auto& r1 = requirements.get(testId1);
    ASSERT_EQ(r1.getRequirementKey(), "TABA1");
    ASSERT_EQ(r1.getText(), testContent1);
  }
  {
    auto& r2 = requirements.get(testId2);
    ASSERT_EQ(r2.getRequirementKey(), "TABB1");
    ASSERT_EQ(r2.getText(), testContent2);
  }
  {
    auto& r3 = requirements.get(testId3);
    ASSERT_EQ(r3.getRequirementKey(), "TABA2");
    ASSERT_EQ(r3.getText(), testContent3);
  }
  {
    auto& r4 = requirements.get(testId4);
    ASSERT_EQ(r4.getRequirementKey(), "TABB2");
    ASSERT_EQ(r4.getText(), testContent4);
  }
}