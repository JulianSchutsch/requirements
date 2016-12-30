#include "req/command/print.hpp"

#include <iostream>

#include "requirements/storage/text.hpp"

#include "generators/latex/requirements.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {

    using PrintFunction = void(*)(::requirements::NodeCollection&, ::requirements::IStorage&, const std::string&);
    using PrintMap = std::map<std::pair<std::string, std::string>, PrintFunction>;

    PrintMap printMap = {{{"latex", "requirements"}, &generators::latex::printRequirements}};

    void processCommand_print(Status& status, const std::vector<std::string>& parameters) {
      if(parameters.size()!=2) {
        throw Exception("Require two parameters for print, format & style");
      }
      ::requirements::storage::Text storage(status.folder, false);
      auto& collection = storage.getNodeCollection();

      std::string format = parameters[0];
      std::string style = parameters[1];

      auto it = printMap.find(std::make_pair(format, style));
      if(it==printMap.end()) {
        throw Exception("Print parameters for format and style not supported");
      }
      it->second(collection, storage, storage.getLatexFolder()+"requirements.tex");
    }
  }
}