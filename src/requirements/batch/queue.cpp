#include "requirements/batch/queue.hpp"

#include <thread>

#include <requirements/exception.hpp>
#include "requirements/annotations/parser.hpp"
#include "requirements/status.hpp"
#include "requirements/icommand.hpp"
#include "requirements/batch/response.hpp"

namespace requirements {
  namespace batch {
    
    void Queue::waitForEmptyQueue() {
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
    
    void Queue::enqueue(std::unique_ptr<ICommand> command) {
      std::lock_guard<std::mutex> guard(queueMutex);
      queue.emplace(std::move(command));
      queueCondition.notify_all();
    }
    
    void Queue::parse(Status &status) {
      if (status.folder == "") {
        if (responseFunction) {
          Response response;
          response.status = status.clone();
          responseFunction(std::move(response));
        }
        return;
      }
      auto storage = status.openStorage();
      ::requirements::annotations::ParserResult result;
      ::requirements::annotations::parse(*storage, result);
      if (responseFunction) {
        Response response;
        response.status = status.clone();
        response.shortcuts = std::move(result.shortcuts);
        response.errors = std::move(result.errors);
        response.requirements = std::move(result.requirements);
        response.sections = std::move(result.sections);
        response.nodeCollection = storage->getNodeCollection().clone();
        responseFunction(std::move(response));
      }
    }
    
    bool Queue::processQueue(Status &status, std::unique_lock<std::mutex> &guard) {
      bool requiredParse = !queue.empty();
      while (!queue.empty()) {
        auto &top = queue.front();
        guard.unlock();
        try {
          top->execute(status);
        } catch(::requirements::Exception& e) {
          switch(e.getKind()) {
            case Exception::Kind::Internal:
              status.messageFunction(Status::MessageKind::InternalError, e.getReason(), e.getParameters());
              break;
            case Exception::Kind::User:
              status.messageFunction(Status::MessageKind::UserError, e.getReason(), e.getParameters());
              break;
            case Exception::Kind::Other:
              status.messageFunction(Status::MessageKind::OtherError, e.getReason(), e.getParameters());
              break;
          }
        }
        guard.lock();
        queue.pop();
      }
      if (requiredParse) {
        parse(status);
      }
      return requiredParse;
    }
    
    void Queue::wait() {
      std::unique_lock<std::mutex> guard(queueMutex);
      if(!processQueue(status, guard)) {
        queueCondition.wait(guard);
      }
    }
    
    void Queue::notify() {
      std::lock_guard<std::mutex> guard(queueMutex);
      queueCondition.notify_all();
    }
    
    Queue::Queue(ResponseFunction a_responseFunction,
                 Status::MessageFunction a_messageFunction,
                 Status::EditFunction a_editFunction,
                 const std::string &a_statusFilename)
      : responseFunction(a_responseFunction)
      , statusFilename(a_statusFilename) {
      status.load(statusFilename);
      status.messageFunction = a_messageFunction;
      status.editFunction = a_editFunction;
    }
    
    void Queue::finish() {
      {
        std::unique_lock<std::mutex> guard(queueMutex);
        processQueue(status, guard);
      }
      status.save(statusFilename);
    }
    
  }
}