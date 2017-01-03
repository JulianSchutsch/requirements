#include "req/command/print.hpp"

#include <iostream>

#include "requirements/storage/text.hpp"

#include "annotations/parser.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    
    void processCommand_print(Status& status, const std::vector<std::string>& parameters) {
      ::requirements::storage::Text storage(status.folder, false);
      ::annotations::ParserResult parsed;
      if(::annotations::parse(storage, parsed)) {
        std::cout<<"No issues found during parsing"<<std::endl;
        return;
      }
      std::cout<<"Some issues found during parsing"<<std::endl;
      for(const auto& error: parsed.errors) {
        std::cout<<::requirements::id_to_string(error.first)<<" -> "<<error.second<<std::endl;
      }
    }
  }
}