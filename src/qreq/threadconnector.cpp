#include "qreq/threadconnector.hpp"
#include <functional>
#include <iostream>

namespace qreq{

ThreadConnector::ThreadConnector(){
  _isnew=false;
  _batchthread=nullptr;
}

ThreadConnector::~ThreadConnector(){
}

void ThreadConnector::init(){
  auto bret = std::bind(&ThreadConnector::batch_ret,this,std::placeholders::_1);
  auto bmes = std::bind(&ThreadConnector::batch_message,this,std::placeholders::_1,std::placeholders::_2, std::placeholders::_3);
  auto efun = std::bind(&ThreadConnector::batch_edit,this,std::placeholders::_1);
  _batchthread=new batch::Thread(bret,bmes,efun);
}

void ThreadConnector::send_command(std::string command){
  _batchthread->enqueue(commands::assembleFromString(command));
}

void ThreadConnector::batch_ret(batch::Response&& bres){
  _conn_mutex.lock();
  _nodeCollection=std::move(bres.nodeCollection);
  _errors=std::move(bres.errors);
  _status=std::move(bres.status);
  _shortcuts=std::move(bres.shortcuts);
  _sections=std::move(bres.sections);
  _requirements=std::move(bres.requirements);
  _isnew=true;
  _conn_mutex.unlock();
  std::cout << "batch_ret" << std::endl;
}

void ThreadConnector::batch_message(Status::MessageKind kind, std::string const& message, const std::vector<std::string>& parameters){
  (void)kind;
  (void)message;
  (void)parameters;
  std::cout << "batch_message" << std::endl;
}

bool ThreadConnector::is_new(){
  _conn_mutex.lock();
  bool retval=_isnew;
  _isnew=false;
  _conn_mutex.unlock();
  return retval;
}

}
