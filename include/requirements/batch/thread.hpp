#pragma once

#include <thread>

#include "requirements/batch/queue.hpp"

namespace requirements {

  class Status;
  class ICommand;

  namespace batch {
    
    struct Response;
    
    class Thread final {
    private:
      volatile bool terminated = false;
      Queue queue;
      std::thread thread;

      void mainloop();
      
    public:
      void waitForEmptyQueue() { queue.waitForEmptyQueue(); }
      
      void enqueue(std::unique_ptr<ICommand> command) { queue.enqueue(std::move(command)); }
      void enqueue(std::vector<std::unique_ptr<ICommand>>&& commands) { queue.enqueue(std::move(commands)); }
      
      Thread(Queue::ResponseFunction a_responseFunction,
             Status::MessageFunction a_messageFunction,
             Status::EditFunction a_editFunction,
             const std::string &a_statusFilename = "");
      
      ~Thread();
    };
  }
}