#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace requirements {

  class Status;
  class ICommand;

  namespace batch {
    
    class BatchResponse;
    
    class BatchThread final {
    private:
      using ResponseFunction = std::function<void(BatchResponse &&)>;
      ResponseFunction responseFunction;
      std::string statusFilename;
      std::thread thread;
      std::mutex queueMutex;
      std::condition_variable queueCondition;
      std::queue<std::unique_ptr<ICommand>> queue;
      volatile bool terminated = false;
      
      void mainloop();
      
      void parse(Status &status);
      
      void processQueue(Status &status, std::unique_lock<std::mutex> &guard);
    
    public:
      void waitForEmptyQueue();
      
      void enqueue(std::unique_ptr<ICommand> command);
      
      BatchThread(ResponseFunction a_responseFunction, const std::string &a_statusFilename = "");
      
      ~BatchThread();
    };
  }
}