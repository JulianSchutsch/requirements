#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

#include "util/path.hpp"
#include "util/process.hpp"
#include "util/stringfile.hpp"

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
    throw ::requirements::Exception("Failed to run editor");
  }
  boost::filesystem::remove_all(tempFilename);
}

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
      (void)kind;
      std::cout<<msg<<std::endl;
    },
    &editCallback,
    util::getConfigPath()+"/.req_status.xml");
  
  batchThread.enqueue(commands::assembleFromString(commandStr));
  
  return 0;
}