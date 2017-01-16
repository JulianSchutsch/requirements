#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include <boost/filesystem.hpp>

#include "test/folder.hpp"

#include "requirements/batch/thread.hpp"
#include "requirements/batch/response.hpp"
#include "requirements/status.hpp"

namespace test {

  using namespace ::requirements;

  class BatchThread final {
  private:
    std::queue<batch::Response> responseQueue;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    std::string statusFile;
    ::test::UniqueFolder folder;
    void responseFunction(batch::Response&& r) {
      std::lock_guard<std::mutex> guard(responseMutex);
      responseQueue.emplace(std::move(r));
      responseCondition.notify_all();
    }
  public:
    std::unique_ptr<batch::Thread> batch;
    BatchThread() {
      statusFile = folder.getName() + "_status.xml";
      {
        Status status;
        status.folder = folder.getName();
        status.save(statusFile);
      }
      batch.reset(new batch::Thread(
        std::bind(&BatchThread::responseFunction, this, std::placeholders::_1),
        [](Status::MessageKind, const std::string&){},
        [](NodePtr){},
        statusFile));
    }
    ~BatchThread() {
      batch.reset();
      boost::filesystem::remove(statusFile);
    }
    batch::Response wait() {
      batch->waitForEmptyQueue();
      std::unique_lock<std::mutex> guard(responseMutex);
      if(!responseQueue.empty()) {
        while(responseQueue.size()!=1) {
          responseQueue.pop();
        }
        auto result = std::move(responseQueue.front());
        responseQueue.pop();
        return std::move(result);
      }
      do {
        responseCondition.wait(guard);
      } while(responseQueue.empty());
      while(responseQueue.size()!=1) {
        responseQueue.pop();
      }
      auto result = std::move(responseQueue.front());
      responseQueue.pop();
      return std::move(result);
    }
  };
}
