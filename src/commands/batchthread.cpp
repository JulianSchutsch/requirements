#include "commands/batchthread.hpp"

#include "annotations/parser.hpp"

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
    if(status.folder=="") {
      return;
    }
    auto storage = status.openStorage();
    ::annotations::ParserResult result;
    ::annotations::parse(*storage, result);
    if(responseFunction) {
      BatchResponse response;
      response.shortcuts = std::move(result.shortcuts);
      response.errors = std::move(result.errors);
      response.requirements = std::move(result.requirements);
      response.sections = std::move(result.sections);
      response.nodeCollection = storage->getNodeCollection().clone();
      responseFunction(std::move(response));
    }
  }

  void BatchThread::processQueue(Status& status, std::unique_lock<std::mutex>& guard) {
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
  
  void BatchThread::mainloop() {
    Status status;
    status.load(statusFilename);
    std::unique_lock<std::mutex> guard(queueMutex);
    processQueue(status, guard);
    while(!terminated) {
      queueCondition.wait(guard);
      processQueue(status, guard);
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