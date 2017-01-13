#include "commands/batchthread.hpp"

#include "commands/status.hpp"
#include "commands/icommand.hpp"
#include "commands/batchresponse.hpp"

namespace commands {
  
  void BatchThread::enqueue(std::unique_ptr<ICommand> command) {
    std::lock_guard<std::mutex> guard(queueMutex);
    queue.emplace(std::move(command));
    queueCondition.notify_all();
  }
  
  void BatchThread::parse(Status& status) {
    auto storage = status.openStorage();
    
  }
  
  void BatchThread::mainloop() {
    Status status;
    status.load(statusFilename);
    std::unique_lock<std::mutex> guard(queueMutex);
    while(!terminated) {
      queueCondition.wait(guard);
      bool requiredParse = !queue.empty();
      while(!queue.empty()) {
        auto& top = queue.front();
        guard.unlock();
        top->execute(status);
        guard.lock();
        queue.pop();
      }
      if(requiredParse) {
        parse(status);
      }
    }
    status.save(statusFilename);
  }
  
  BatchThread::BatchThread(ResponseFunction a_responseFunction, const std::string& a_statusFilename)
    : responseFunction(a_responseFunction)
    , statusFilename(a_statusFilename) {
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