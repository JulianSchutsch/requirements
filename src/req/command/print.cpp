#include "req/command/print.hpp"

#include <iostream>
#include <fstream>

#include "requirements/storage/text.hpp"

#include "annotations/parser.hpp"

#include "req/status.hpp"

#include "generators/latex/requirements.hpp"

namespace req {
  namespace command {
    
    void processCommand_print(Status& status, const std::vector<std::string>& parameters) {
      (void)parameters;
      ::requirements::storage::Text storage(status.folder, false);
      ::annotations::ParserResult parsed;
      if(!::annotations::parse(storage, parsed)) {
        std::cout<<"Some issues found during parsing"<<std::endl;
        for(auto node: storage.getNodeCollection()) {
          auto id = node->getId();
          if(parsed.errors->has(id)) {
            std::cout<<::requirements::id_to_string(id)<<" -> "<<parsed.errors->get(id)<<std::endl;
          }
        }
        return;
      }
      std::fstream file(status.folder+"/latex/requirements.tex", std::fstream::out);
      if(!file) {
        std::cout<<"Could not open target file"<<std::endl;
        return;
      }
      ::generators::latex::printRequirements(*parsed.sections, *parsed.requirements, file);
      std::cout<<"Generated requirements document in requirements "<<status.folder<<"/latex/requirements.tex"<<std::endl;
    }
  }
}