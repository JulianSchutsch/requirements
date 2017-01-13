#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include "commands/batchthread.hpp"
#include "commands/batchresponse.hpp"
#include "commands/status.hpp"

namespace test {
  class BatchThread {
    std::queue<::commands::BatchResponse> responseQueue;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    void responseFunction(::commands::BatchResponse&& r) {
      std::lock_guard<std::mutex> guard(responseMutex);
      responseQueue.emplace(std::move(r));
      responseCondition.notify_all();
    }
  private:
    std::unique_ptr<::commands::BatchThread> batch;
    BatchThread() {
      ::test::UniqueFolder folder;
      auto statusFile = folder + "/status.xml";
      {
        Status status;
        status.folder = folder.getName();
        status.save(folder + statusFile);
      }
      batch.reset(new ::commands::BatchThread(statusFile, std::function(&BatchThread::responseFunction, this)));
    }
    BatchResponse wait() {
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