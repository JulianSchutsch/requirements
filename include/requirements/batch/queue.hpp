#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <condition_variable>
#include <memory>
#include <queue>

#include "requirements/status.hpp"
#include "requirements/icommand.hpp"

namespace requirements {
  
  class Status;
  class ICommand;
  
  namespace batch {
    
    class Response;
    
    class Queue final {
    public:
      using ResponseFunction = std::function<void(Response&&)>;
      using MessageFunction = ICommand::MessageFunction;
    private:
      ResponseFunction responseFunction;
      MessageFunction messageFunction;
      Status status;
      std::string statusFilename;
      std::mutex queueMutex;
      std::condition_variable queueCondition;
      std::queue<std::unique_ptr<ICommand>> queue;
      
      void parse(Status &status);
      
      bool processQueue(Status &status, std::unique_lock<std::mutex> &guard);
    
    public:
      void waitForEmptyQueue();
  
      void enqueue(std::unique_ptr<ICommand> command);
      
      void notify();
      void wait();
      void finish();
      
      Queue(ResponseFunction a_responseFunction, MessageFunction a_messageFunction, const std::string &a_statusFilename = "");
    };
  }
}