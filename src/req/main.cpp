#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <requirements/batch/response.hpp>

#include "util/path.hpp"
#include "util/process.hpp"
#include "util/stringfile.hpp"
#include "util/formatstring.hpp"

#include "requirements/batch/thread.hpp"
#include "requirements/commands/command.hpp"
#include "requirements/exception.hpp"

using namespace ::requirements;

void editCallback(NodePtr node) {
  boost::filesystem::path tempFilename = boost::filesystem::unique_path();
  {
    std::fstream f(tempFilename.native(), std::fstream::out);
    f<<node->getContent();
  }
  if(util::runProcess("/bin/nano", {tempFilename.native()})) {
    node->updateContent(util::readFileToString(tempFilename.native()));
  } else {
    throw ::requirements::Exception(Exception::Kind::Other, "Failed to run editor");
  }
  boost::filesystem::remove_all(tempFilename);
}

void printInternalError(const std::string& format, const std::vector<std::string>& parameters) {
  std::cout<<"*** Internal error ***"<<std::endl;
  std::cout<<"This kind of error is worth a bug report, it is Not your fault."<<std::endl;
  std::cout<<"Message: "<<util::formatString(format, parameters)<<std::endl;
}

void printUserError(const std::string& format, const std::vector<std::string>& parameters) {
  std::cout<<"Error detected:"<<util::formatString(format, parameters)<<std::endl;
}

std::string joinArgs(int argc, char** args) {
  std::string commandStr;
  for(int i=1;i<argc;++i) {
    if(!commandStr.empty()) {
      commandStr+=" ";
    }
    commandStr+=args[i];
  }
  return commandStr;
}

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  
  auto commandStr = joinArgs(argc, args);

  batch::Thread batchThread(
    [](batch::Response&& r){
      if(!r.status->selections[0].empty()) {
        std::cout<<"Selected: ";
        for(auto selected: r.status->selections[0]) {
          std::cout<<id_to_string(selected)<<" ";
        }
        std::cout<<std::endl;
      }
    },
    [](Status::MessageKind kind, const std::string& msg, const std::vector<std::string>& parameters) {
      switch(kind)
      {
        case Status::MessageKind::InternalError:
          printInternalError(msg, parameters);
          break;
        case Status::MessageKind::UserError:
        case Status::MessageKind::OtherError:
          printUserError(msg, parameters);
          break;
        case Status::MessageKind::Content:
        case Status::MessageKind::Message:
          std::cout<<util::formatString(msg, parameters)<<std::endl;
          break;
      }
    },
    &editCallback,
    util::getConfigPath()+"/.req_status.xml");
  
  try {
    batchThread.enqueue(commands::assembleFromString(commandStr));
  } catch(::requirements::Exception& e) {
    switch(e.getKind()) {
      case Exception::Kind::Internal: {
        printInternalError(e.getReason(), e.getParameters());
        break;
      }
      case Exception::Kind::User:
      case Exception::Kind::Other:
        printUserError(e.getReason(), e.getParameters());
        break;
    }
  }
  
  return 0;
}