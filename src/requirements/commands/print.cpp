#include "requirements/commands/print.hpp"

#include <fstream>

#include "requirements/status.hpp"
#include "requirements/exception.hpp"
#include "requirements/annotations/parser.hpp"
#include "requirements/generators/latex/requirements.hpp"
#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    void Print::execute(Status &status) {
      auto storage = status.openStorage();
      annotations::ParserResult parsed;
      if(!annotations::parse(*storage, parsed)) {
        throw Exception("Issues during parsing, cannot print");
      }
      std::fstream file(status.folder+"/latex/requirements.tex", std::fstream::out);
      if(!file) {
        throw Exception("Could not open target file for print");
      }
      generators::latex::printRequirements(*parsed.sections, *parsed.requirements, file);
      status.messageFunction(Status::MessageKind::Message, "Generated requirements document in requirements "+status.folder+"/latex/requirements.tex");
  
    }
    
    Print::Print() {}
    
    Print::Print(Parser& parser) {
      if(parser.getRemaining()!="") {
        throw Exception("No parameters for print supported");
      }
    }
  }
}