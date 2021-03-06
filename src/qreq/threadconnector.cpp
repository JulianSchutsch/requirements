#include "qreq/threadconnector.hpp"
#include <functional>
#include <iostream>
#include <memory>

#include "util/path.hpp"
#include "util/formatstring.hpp"

#include "requirements/commands/null.hpp"

namespace qreq{

ThreadConnector::ThreadConnector()
  : _batchthread(
    std::bind(&ThreadConnector::batch_ret,this,std::placeholders::_1),
    std::bind(&ThreadConnector::batch_message,this,std::placeholders::_1,std::placeholders::_2, std::placeholders::_3),
    std::bind(&ThreadConnector::batch_edit,this,std::placeholders::_1),
    util::getConfigPath()+".req_status.xml") {
  _batchthread.enqueue(std::make_unique<::requirements::commands::Null>());
}

void ThreadConnector::send_command(std::string command){
  _batchthread.enqueue(commands::assembleFromString(command));
}

void ThreadConnector::batch_ret(batch::Response&& bres){
  {
    std::lock_guard<std::mutex> guard(_conn_mutex);
    _response = std::move(bres);
    newResponse = true;
  }
}

void ThreadConnector::batch_message(Status::MessageKind kind, std::string const& message, const std::vector<std::string>& parameters){
  std::lock_guard<std::mutex> guard(_conn_mutex);
  messages.emplace_back(kind, message, parameters);
}

bool ThreadConnector::consumeResponse(::requirements::batch::Response& target){
  std::lock_guard<std::mutex> guard(_conn_mutex);
  if(newResponse) {
    target = std::move(_response);
    newResponse = false;
    return true;
  }
  return false;
}

std::list<BatchMessage> ThreadConnector::consumeMessages() {
  std::list<BatchMessage> result;
  {
    std::lock_guard<std::mutex> guard(_conn_mutex);
    result = std::move(messages);
  }
  return result;
}

}
