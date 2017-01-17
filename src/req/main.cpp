#include <string>
#include <iostream>

#include "util/path.hpp"

#include "requirements/batch/thread.hpp"
#include "requirements/commands/command.hpp"

using namespace ::requirements;

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  
  std::string commandStr;
  for(int i=1;i<argc;++i) {
    if(!commandStr.empty()) {
      commandStr+=" ";
    }
    commandStr+=args[i];
  }

  batch::Thread batchThread(
    [](batch::Response&&){},
    [](Status::MessageKind kind, const std::string& msg) {
      std::cout<<msg<<std::endl;
    },
    [](NodePtr) {
      std::cout<<"Attempt to edit"<<std::endl;
    },
    util::getConfigPath()+"/.req_status.xml");
  
  batchThread.enqueue(commands::assembleFromString(commandStr));
  
  return 0;
}