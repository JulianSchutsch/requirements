#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <condition_variable>
#include <memory>
#include <queue>

#include "requirements/status.hpp"
#include "requirements/icommand.hpp"
#include "requirements/node.hpp"

namespace requirements {
  
  class Status;
  class ICommand;
  class Exception;
  
  namespace batch {
    
    class Response;
    
    class Queue final {
    public:
      using ResponseFunction = std::function<void(Response&&)>;
    private:
      ResponseFunction responseFunction;
      Status status;
      std::string statusFilename;
      std::mutex queueMutex;
      std::condition_variable queueCondition;
      std::queue<std::unique_ptr<ICommand>> queue;
      
      void parse(Status &status);
      
      bool processQueue(Status &status, std::unique_lock<std::mutex> &guard);

      void forwardException(Exception& e);
    
    public:
      void waitForEmptyQueue();
  
      void enqueue(std::unique_ptr<ICommand> command);
      void enqueue(std::vector<std::unique_ptr<ICommand>>&& commands);
      
      // Wakeup the Queue-Thread
      void notify();
      // The wait function waits for new commands being inserted into the queue.
      // The terminated flag has to be passed since the locking mechanism is controlled by the queue and therefore
      // there is a rare chance of terminated being set just before wait expects a condition and just after
      // the condition has been triggered (and is therefore now lost)
      void wait(volatile bool& terminated);
      void finish();
      
      Queue(ResponseFunction a_responseFunction,
            Status::MessageFunction a_messageFunction,
            Status::EditFunction a_editFunction,
            const std::string &a_statusFilename = "");
    };
  }
}