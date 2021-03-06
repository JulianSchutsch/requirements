#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include <boost/filesystem.hpp>

#include "test/folder.hpp"

#include "requirements/batch/thread.hpp"
#include "requirements/batch/response.hpp"
#include "requirements/status.hpp"
#include "requirements/node.hpp"

namespace test {

  using namespace ::requirements;

  class BatchThread final {
  private:
    std::queue<batch::Response> responseQueue{};
    std::mutex responseMutex{};
    std::condition_variable responseCondition{};
    std::string statusFile{};
    void responseFunction(batch::Response&& r) {
      std::lock_guard<std::mutex> guard(responseMutex);
      responseQueue.emplace(std::move(r));
      responseCondition.notify_all();
    }
    void processMessage(Status::MessageKind kind, const std::string& msg, const std::vector<std::string>& parameters) {
      switch(kind) {
        case Status::MessageKind::Content:
          msg_content(msg, parameters);
          break;
        default:
          break;
      }
    }
  public:
    ::test::UniqueFolder folder{};
    std::function<void(const std::string& content, const std::vector<std::string>& parameters)> msg_content{};
    std::function<void(::requirements::NodePtr node)> msg_edit{};
    std::unique_ptr<batch::Thread> batch{};
    BatchThread() {
      statusFile = folder.getName() + "_status.xml";
      {
        Status status;
        status.folder = folder.getName();
        status.save(statusFile);
      }
      batch.reset(new batch::Thread(
        std::bind(&BatchThread::responseFunction, this, std::placeholders::_1),
        std::bind(&BatchThread::processMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        [this](NodePtr node){if(msg_edit) {msg_edit(node);}},
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
        while(responseQueue.size()>1) {
          responseQueue.pop();
        }
        auto result = std::move(responseQueue.front());
        responseQueue.pop();
        return result;
      }
      do {
        responseCondition.wait(guard);
      } while(responseQueue.empty());
      while(responseQueue.size()>1) {
        responseQueue.pop();
      }
      auto result = std::move(responseQueue.front());
      responseQueue.pop();
      return result;
    }
  };
}
