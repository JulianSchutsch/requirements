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
  std::function<void(batch::Response&&)> bret = std::bind(&ThreadConnector::batch_ret,this,std::placeholders::_1);
  std::function<void(Status::MessageKind,std::string const&)> bmes = std::bind(&ThreadConnector::batch_message,this,std::placeholders::_1,std::placeholders::_2);
  std::function<void(NodePtr)> efun = std::bind(&ThreadConnector::batch_edit,this,std::placeholders::_1);
  _batchthread=new batch::Thread(bret,bmes,efun);
}

void ThreadConnector::send_command(std::string command){
  _batchthread->enqueue(commands::assembleFromString(command));
}

void ThreadConnector::batch_ret(batch::Response&& bres){
  (void) bres;
  std::cout << "batch_ret" << std::endl;
}

void ThreadConnector::batch_message(Status::MessageKind kind, std::string const& message){
  (void)kind;
  (void)message;
  std::cout << "batch_message" << std::endl;
}

}
