#include "req/command/folder.hpp"

#include <boost/filesystem.hpp>

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_folder(Status& status, const std::vector<std::string>& parameters) {
      
      if(parameters.size()!=1) {
        throw Exception("Requires parameter: Folder");
      }

      boost::filesystem::path relative(parameters[0].c_str());
      boost::filesystem::path absolute = boost::filesystem::absolute(relative, boost::filesystem::current_path());
      status.folder = absolute.native();
    }
  }
}