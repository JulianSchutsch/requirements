#include "util/process.hpp"

#include <iostream>

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

namespace util {
  
  bool runProcess(const std::string& command, const std::vector<std::string>& parameters) {
    
    std::vector<std::string> tParameters;
    std::vector<char*> cParameters;
    
    tParameters.reserve(parameters.size()+1);
    cParameters.reserve(parameters.size()+2);
    tParameters.emplace_back(command);
    cParameters.emplace_back(&tParameters[0][0]);
    
    for(std::size_t i=0;i<parameters.size();++i) {
      tParameters.emplace_back(parameters[i]);
      cParameters.emplace_back(&tParameters[i+1][0]);
    }
    cParameters.emplace_back(nullptr);
    
    auto pid = fork();
    
    switch(pid) {
    case -1:
      exit(1);
    case 0:
    {
      execv(command.c_str(), &cParameters[0]);
      int e = errno;
      char buffer[1024]={0};
      std::cout<<strerror_r(e, buffer, 1024)<<std::endl;
      std::cout<<buffer<<std::endl;
      exit(1);
    }
    default:
      int status;
      waitpid(pid, &status, 0);
      return WEXITSTATUS(status)==0;
    }
    
  }
  
}