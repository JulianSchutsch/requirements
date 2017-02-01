#pragma once

#include <mutex>
#include <string>
#include <vector>
#include <memory>

#include "requirements/batch/thread.hpp"
#include "requirements/batch/response.hpp"
#include "requirements/commands/command.hpp"
#include "requirements/exception.hpp"
#include "requirements/nodecollection.hpp"

using namespace ::requirements;

namespace qreq{

class ThreadConnector final {
private:
  std::mutex _conn_mutex;
  // The response contains all possible results from the batch thread
  bool newResponse = false;
  batch::Response _response;
  void batch_ret(batch::Response&& bres);
  void batch_message(Status::MessageKind kind, std::string const& message, const std::vector<std::string>& parameters);
  void batch_edit(NodePtr node){(void)node;}
public:
  batch::Thread _batchthread;
  ThreadConnector();
  void send_command(std::string command);
  // If new data has been received from the batch thread, this function returns true and moves all
  // data to the "target". Otherwise returns false.
  bool consumeResponse(batch::Response& target);
};

}
