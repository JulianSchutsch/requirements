#include "requirements/batch/batchthread.hpp"

#include "requirements/annotations/parser.hpp"

#include "requirements/status.hpp"
#include "requirements/icommand.hpp"
#include "requirements/batch/batchresponse.hpp"

namespace requirements {
  namespace batch {
    
    void BatchThread::waitForEmptyQueue() {
      for (;;) {
        {
          std::lock_guard<std::mutex> guard(queueMutex);
          if (queue.empty()) {
            break;
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
    
    void BatchThread::enqueue(std::unique_ptr<ICommand> command) {
      std::lock_guard<std::mutex> guard(queueMutex);
      queue.emplace(std::move(command));
      queueCondition.notify_all();
    }
    
    void BatchThread::parse(Status &status) {
      if (status.folder == "") {
        if (responseFunction) {
          BatchResponse response;
          response.status = status.clone();
          responseFunction(std::move(response));
        }
        return;
      }
      auto storage = status.openStorage();
      ::requirements::annotations::ParserResult result;
      ::requirements::annotations::parse(*storage, result);
      if (responseFunction) {
        BatchResponse response;
        response.status = status.clone();
        response.shortcuts = std::move(result.shortcuts);
        response.errors = std::move(result.errors);
        response.requirements = std::move(result.requirements);
        response.sections = std::move(result.sections);
        response.nodeCollection = storage->getNodeCollection().clone();
        responseFunction(std::move(response));
      }
    }
    
    void BatchThread::processQueue(Status &status, std::unique_lock<std::mutex> &guard) {
      bool requiredParse = !queue.empty();
      while (!queue.empty()) {
        auto &top = queue.front();
        guard.unlock();
        top->execute(status);
        guard.lock();
        queue.pop();
      }
      if (requiredParse) {
        parse(status);
      }
    }
    
    void BatchThread::mainloop() {
      Status status;
      status.load(statusFilename);
      std::unique_lock<std::mutex> guard(queueMutex);
      processQueue(status, guard);
      while (!terminated) {
        queueCondition.wait(guard);
        processQueue(status, guard);
      }
      status.save(statusFilename);
    }
    
    BatchThread::BatchThread(ResponseFunction a_responseFunction, const std::string &a_statusFilename)
      : responseFunction(a_responseFunction), statusFilename(a_statusFilename) {
      thread = std::thread(&BatchThread::mainloop, this);
    }
    
    BatchThread::~BatchThread() {
      {
        std::unique_lock<std::mutex> guard(queueMutex);
        terminated = true;
        queueCondition.notify_all();
      }
      thread.join();
    }
    
  }
}