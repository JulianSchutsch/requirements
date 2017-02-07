#include <gtest/gtest.h>

#include "test/batchthread.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/setcontent.hpp"
#include "requirements/commands/lastof.hpp"

using namespace requirements;
using namespace requirements::commands;

TEST(AnnotationAcceptances, SimpleTree) {
  ::test::BatchThread b;
  auto reqs = generateRandomId();
  auto req1 = generateRandomId();
  auto req11 = generateRandomId();
  auto req12 = generateRandomId();
  auto accs = generateRandomId();
  auto acc1 = generateRandomId();
  auto acc2 = generateRandomId();
  b.batch->enqueue(std::make_unique<New>(reqs));
  b.batch->enqueue(std::make_unique<SetContent>(reqs, "requirements: R Requirements"));
  b.batch->enqueue(std::make_unique<New>(req1));
  b.batch->enqueue(std::make_unique<LastOf>(req1, reqs));
  b.batch->enqueue(std::make_unique<New>(req11));
  b.batch->enqueue(std::make_unique<LastOf>(req11, req1));
  b.batch->enqueue(std::make_unique<New>(req12));
  b.batch->enqueue(std::make_unique<LastOf>(req12, req1));
  b.batch->enqueue(std::make_unique<New>(accs));
  b.batch->enqueue(std::make_unique<SetContent>(accs, "acceptance: R Acceptance"));
  b.batch->enqueue(std::make_unique<New>(acc1));
  b.batch->enqueue(std::make_unique<SetContent>(acc1, "accepts: "+id_to_string(req12)));
  b.batch->enqueue(std::make_unique<LastOf>(acc1, accs));
  b.batch->enqueue(std::make_unique<New>(acc2));
  b.batch->enqueue(std::make_unique<SetContent>(acc2, "accepts: "+id_to_string(req1)));
  b.batch->enqueue(std::make_unique<LastOf>(acc2, accs));
  auto response = b.wait();
  auto& acceptances = *response.acceptances;
  {
    auto rootAcceptances = acceptances.getRootEntries();
    ASSERT_EQ(rootAcceptances.size(), 2);
    ASSERT_EQ(rootAcceptances.front(), acc1);
    ASSERT_EQ(rootAcceptances.back(), acc2);
  }
  ASSERT_EQ(acceptances.has(acc1), true);
  ASSERT_EQ(acceptances.has(acc2), true);
  ASSERT_NE(acceptances.has(accs), true);
  ASSERT_NE(acceptances.has(reqs), true);
  ASSERT_NE(acceptances.has(req1), true);
  ASSERT_NE(acceptances.has(req11), true);
  ASSERT_NE(acceptances.has(req12), true);
  {
    auto& acceptance1 = acceptances.get(acc1);
    ASSERT_EQ(acceptance1.getKey(), "R2");
    auto& accepts1 = acceptance1.getAccepts();
    ASSERT_EQ(accepts1.size(), 1);
    ASSERT_EQ(accepts1.front(), req12);
  }
  {
    auto& acceptance2 = acceptances.get(acc2);
    ASSERT_EQ(acceptance2.getKey(), "R3");
    auto& accepts2 = acceptance2.getAccepts();
    ASSERT_EQ(accepts2.size(), 1);
    ASSERT_EQ(accepts2.front(), req1);
  }
  auto& req = *response.requirements;
  {
    ASSERT_EQ(req.has(req1), true);
    auto& areq1 = req.get(req1);
    ASSERT_EQ(areq1.getKey(), "R1");
    ASSERT_EQ(areq1.isCoveredByAcceptance(), true);
    ASSERT_EQ(areq1.isTreeCoveredByAcceptance(), false);
  }
  {
    ASSERT_EQ(req.has(req11), true);
    auto& areq11 = req.get(req11);
    ASSERT_EQ(areq11.getKey(), "R1.1");
    ASSERT_EQ(areq11.isCoveredByAcceptance(), false);
    ASSERT_EQ(areq11.isTreeCoveredByAcceptance(), false);
  }
  {
    ASSERT_EQ(req.has(req12), true);
    auto& areq12 = req.get(req12);
    ASSERT_EQ(areq12.getKey(), "R1.2");
    ASSERT_EQ(areq12.isCoveredByAcceptance(), true);
    ASSERT_EQ(areq12.isTreeCoveredByAcceptance(), true);
  }

}