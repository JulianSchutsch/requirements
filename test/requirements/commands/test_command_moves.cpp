#include <gtest/gtest.h>

#include "test/batchthread.hpp"

#include "requirements/commands/command.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/up.hpp"
#include "requirements/commands/down.hpp"
#include "requirements/commands/firstof.hpp"
#include "requirements/commands/lastof.hpp"
#include "requirements/commands/nextto.hpp"
#include "requirements/commands/previousto.hpp"

using namespace requirements;

#define TWOELEMENTS\
  test::BatchThread b;\
  auto id1 = generateRandomId();\
  auto id2 = generateRandomId();\
  b.batch->enqueue(std::make_unique<commands::New>(id1));\
  b.batch->enqueue(std::make_unique<commands::New>(id2));\
  do {} while(0)

#define THREEELEMENTS\
  test::BatchThread b;\
  auto id1 = generateRandomId();\
  auto id2 = generateRandomId();\
  auto id3 = generateRandomId();\
  b.batch->enqueue(std::make_unique<commands::New>(id1));\
  b.batch->enqueue(std::make_unique<commands::New>(id2));\
  b.batch->enqueue(std::make_unique<commands::New>(id3));\
  do {} while(0)
  

TEST(Commands, Command_Up) {
  TWOELEMENTS;
  b.batch->enqueue(std::make_unique<commands::Up>(id2));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children.front()->getId(), id2);
  ASSERT_EQ(children.back()->getId(), id1);
}

TEST(Commands, Command_Up_Console) {
  TWOELEMENTS;
  b.batch->enqueue(commands::assembleFromString("select "+::requirements::id_to_string(id2)));
  b.batch->enqueue(commands::assembleFromString("up"));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children.front()->getId(), id2);
  ASSERT_EQ(children.back()->getId(), id1);
}

TEST(Commands, Command_Down) {
  TWOELEMENTS;
  b.batch->enqueue(std::make_unique<::commands::Down>(id1));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children.front()->getId(), id2);
  ASSERT_EQ(children.back()->getId(), id1);
}

TEST(Commands, Command_Down_Console) {
  TWOELEMENTS;
  b.batch->enqueue(::commands::assembleFromString("select "+::requirements::id_to_string(id1)));
  b.batch->enqueue(::commands::assembleFromString("down"));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children.front()->getId(), id2);
  ASSERT_EQ(children.back()->getId(), id1);
}

TEST(Commands, Command_LastOf) {
  TWOELEMENTS;
  b.batch->enqueue(std::make_unique<::commands::LastOf>(id1, id2));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id2);
  auto subChildren = children.front()->getChildren();
  ASSERT_EQ(subChildren.size(), 1);
  ASSERT_EQ(subChildren.front()->getId(), id1);
}

TEST(Commands, Command_LastOf_Console) {
  TWOELEMENTS;
  b.batch->enqueue(::commands::assembleFromString("select "+::requirements::id_to_string(id1)));
  b.batch->enqueue(::commands::assembleFromString("lastof "+::requirements::id_to_string(id2)));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id2);
  auto subChildren = children.front()->getChildren();
  ASSERT_EQ(subChildren.size(), 1);
  ASSERT_EQ(subChildren.front()->getId(), id1);
}

TEST(Commands, Command_FirstOf) {
  THREEELEMENTS;
  b.batch->enqueue(std::make_unique<::commands::LastOf>(id2, id1));
  b.batch->enqueue(std::make_unique<::commands::FirstOf>(id3, id1));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id1);
  auto subChildren = children.front()->getChildren();
  ASSERT_EQ(subChildren.size(), 2);
  ASSERT_EQ(subChildren.front()->getId(), id3);
  ASSERT_EQ(subChildren.back()->getId(), id2);
}

TEST(Commands, Command_FirstOf_Console) {
  THREEELEMENTS;
  b.batch->enqueue(::commands::assembleFromString("select "+::requirements::id_to_string(id2)));
  b.batch->enqueue(::commands::assembleFromString("lastof "+::requirements::id_to_string(id1)));
  b.batch->enqueue(::commands::assembleFromString("select "+::requirements::id_to_string(id3)));
  b.batch->enqueue(::commands::assembleFromString("firstof "+::requirements::id_to_string(id1)));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 1);
  ASSERT_EQ(children.front()->getId(), id1);
  auto subChildren = children.front()->getChildren();
  ASSERT_EQ(subChildren.size(), 2);
  ASSERT_EQ(subChildren.front()->getId(), id3);
  ASSERT_EQ(subChildren.back()->getId(), id2);
}

TEST(Commnds, Command_NextTo) {
  THREEELEMENTS;
  b.batch->enqueue(std::make_unique<commands::NextTo>(id1, id3));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 3);
  auto it = children.begin();
  ASSERT_EQ((*it)->getId(), id2);
  ++it;
  ASSERT_EQ((*it)->getId(), id3);
  ++it;
  ASSERT_EQ((*it)->getId(), id1);
}

TEST(Commnds, Command_NextTo_Console) {
  THREEELEMENTS;
  b.batch->enqueue(commands::assembleFromString("select "+id_to_string(id1)));
  b.batch->enqueue(commands::assembleFromString("nextto "+id_to_string(id3)));
  auto response = b.wait();
  auto children = response.nodeCollection->getRootNode()->getChildren();
  ASSERT_EQ(children.size(), 3);
  auto it = children.begin();
  ASSERT_EQ((*it)->getId(), id2);
  ++it;
  ASSERT_EQ((*it)->getId(), id3);
  ++it;
  ASSERT_EQ((*it)->getId(), id1);
}