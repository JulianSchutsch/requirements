#include "commands/batchthread.hpp"

#include <iostream>

#include "commands/icommand.hpp"

namespace commands {
  
  void BatchThread::enqueue(std::unique_ptr<ICommand> command) {
    std::lock_guard<std::mutex> guard(queueMutex);
    queue.emplace(std::move(command));
    queueCondition.notify_all();
  }
  
  void BatchThread::mainloop() {
    std::unique_lock<std::mutex> guard(queueMutex);
    while(!terminated) {
      queueCondition.wait(guard);
      while(!queue.empty()) {
        auto& top = queue.front();
        guard.unlock();
        top->execute();
        guard.lock();
        queue.pop();
      }
    }
  }
  
  BatchThread::BatchThread() {
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