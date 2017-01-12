#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace commands {
  
  class ICommand;
  
  class BatchThread {
  private:
    std::thread thread;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    std::queue<std::unique_ptr<ICommand>> queue;
    volatile bool terminated = false;
    void mainloop();
  public:
    void enqueue(std::unique_ptr<ICommand> command);
    BatchThread();
    ~BatchThread();
  };
}