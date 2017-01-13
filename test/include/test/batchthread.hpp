#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include <boost/filesystem.hpp>

#include "commands/batchthread.hpp"
#include "commands/batchresponse.hpp"
#include "commands/status.hpp"

namespace test {
  class BatchThread final {
  private:
    std::queue<::commands::BatchResponse> responseQueue;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    std::string statusFile;
    ::test::UniqueFolder folder;
    void responseFunction(::commands::BatchResponse&& r) {
      std::lock_guard<std::mutex> guard(responseMutex);
      responseQueue.emplace(std::move(r));
      responseCondition.notify_all();
    }
  public:
    std::unique_ptr<::commands::BatchThread> batch;
    BatchThread() {
      statusFile = folder.getName() + "_status.xml";
      {
        ::commands::Status status;
        status.folder = folder.getName();
        status.save(statusFile);
      }
      batch.reset(new ::commands::BatchThread(std::bind(&BatchThread::responseFunction, this, std::placeholders::_1), statusFile));
    }
    ~BatchThread() {
      batch.release();
      boost::filesystem::remove(statusFile);
    }
    commands::BatchResponse wait() {
      std::unique_lock<std::mutex> guard(responseMutex);
      if(!responseQueue.empty()) {
        auto result = std::move(responseQueue.front());
        responseQueue.pop();
        return std::move(result);
      }
      do {
        responseCondition.wait(guard);
      } while(responseQueue.empty());
      auto result = std::move(responseQueue.front());
      responseQueue.pop();
      return std::move(result);
    }
  };
}
