#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "requirements/batch/thread.hpp"
#include "requirements/batch/response.hpp"
#include "requirements/commands/command.hpp"
#include "requirements/exception.hpp"
#include "requirements/nodecollection.hpp"

using namespace ::requirements;

namespace qreq{

class ThreadConnector{
  //batch::Response _response;
  bool _isnew;
  batch::Thread* _batchthread;
  std::mutex _conn_mutex;
  //std::unique_ptr<NodeCollection> _nodeCollection;
/*other*/ public:
  std::unique_ptr<NodeCollection> _nodeCollection;
  std::unique_ptr<::requirements::Status> _status;
  std::unique_ptr<::requirements::annotations::Errors> _errors;
  std::unique_ptr<::requirements::annotations::Shortcuts> _shortcuts;
  std::unique_ptr<::requirements::annotations::Sections> _sections;
  std::unique_ptr<::requirements::annotations::Requirements> _requirements;
public:
  ThreadConnector();
  ~ThreadConnector();
  void init();
  void send_command(std::string command);
  void batch_ret(batch::Response&& bres);
  void batch_message(Status::MessageKind kind, std::string const& message, const std::vector<std::string>& parameters);
  void batch_edit(NodePtr node){(void)node;}
  bool is_new();
  //std::unique_ptr<NodeCollection> nodeCollection();
};

}
