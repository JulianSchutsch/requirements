#include <iostream>
#include <vector>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "req/status.hpp"
#include "req/command.hpp"

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  
  req::Status status;

  status.load();

  std::vector<std::string> commands;
  commands.reserve(argc-1);
  for(int i=1;i<argc;++i) {
    commands.push_back(args[i]);
  }

  req::processCommand(status, commands);

  status.save();
  
  return 0;
}