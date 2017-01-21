#pragma once

#include "requirements/batch/thread.hpp"
#include "requirements/batch/response.hpp"
#include "requirements/commands/command.hpp"
#include "requirements/exception.hpp"

using namespace ::requirements;

namespace qreq{

class ThreadConnector{
  batch::Response _response;
  bool _isnew;
  batch::Thread* _batchthread;
public:
  ThreadConnector();
  ~ThreadConnector();
  void init();
  void send_command(std::string command);
  void batch_ret(batch::Response&& bres);
  void batch_message(Status::MessageKind kind, std::string const& message);
  void batch_edit(NodePtr node){(void)node;}
};

}
