#include "req/command/print.hpp"

#include <iostream>

#include "requirements/storage/text.hpp"
#include "requirements/nodecollection.hpp"

#include "generators/latex/requirements.hpp"

#include "req/status.hpp"

namespace req {
  namespace command {

    using PrintFunction = void(*)(::requirements::NodeCollection&, ::requirements::IStorage&, const std::string&);
    using PrintMap = std::map<std::pair<std::string, std::string>, PrintFunction>;

    PrintMap printMap = {{{"latex", "requirements"}, &generators::latex::printRequirements}};

    void processCommand_print(Status& status, const std::vector<std::string>& parameters) {
      if(parameters.size()!=2) {
        std::cout<<"Expected 2 parameters for print: target format & style"<<std::endl;
      }
      ::requirements::NodeCollection collection;
      ::requirements::storage::Text storage(collection, status.folder);

      std::string format = parameters[0];
      std::string style = parameters[1];

      auto it = printMap.find(std::make_pair(format, style));
      if(it==printMap.end()) {
        std::cout<<"Print parameters for format and style not supported"<<std::endl;
        return;
      }
      it->second(collection, storage, storage.getLatexFolder()+"requirements.tex");
    }
  }
}