#include "requirements/commands/print.hpp"

#include <fstream>

#include "requirements/status.hpp"
#include "requirements/exception.hpp"
#include "requirements/annotations/parser.hpp"
#include "requirements/generators/latex/requirements.hpp"
#include "requirements/generators/latex/scenes.hpp"
#include "requirements/generators/latex/labels.hpp"
#include "requirements/commands/parser.hpp"
#include "requirements/internal/path.hpp"

namespace requirements {
  namespace commands {

    class PhaseFiles : public ::requirements::generators::IPhaseFiles {
    private:
      std::map<std::string, std::fstream*> files;
    public:
      std::ostream& getOStream(const std::string& phase) override;
      PhaseFiles(const std::string& prefix, const std::string& postfix, annotations::Phases& phases);
      ~PhaseFiles();
    };

    std::ostream& PhaseFiles::getOStream(const std::string& phase) {
      return *files[phase];
    }

    PhaseFiles::PhaseFiles(const std::string& prefix, const std::string& postfix, annotations::Phases& phases) {
      {
        std::string filename = prefix + "/" + postfix;
        files.emplace("", new std::fstream(filename, std::fstream::out));
        if (!files[""]->is_open()) {
          throw Exception(Exception::Kind::User, "Could not open target file %1% for printing.", {filename});
        }
      }
      for(auto& phase: phases) {
        std::string folder = prefix+"/"+phase.second;
        std::string filename = folder+"/"+postfix;
        ::requirements::internal::ensureFolder(folder);
        files.emplace(phase.second, new std::fstream(filename, std::fstream::out));
        if (!files[phase.second]->is_open()) {
          throw Exception(Exception::Kind::User, "Could not open target file %1% for printing.", {filename});
        }
      }
    }

    PhaseFiles::~PhaseFiles() {
      for(auto& file: files) {
        delete file.second;
      }
    }

    void Print::execute(Status &status) {
      annotations::ParserResult parsed;
      auto storage = status.openStorage();
      if(!annotations::parse(*storage, parsed)) {
        throw Exception(Exception::Kind::User, "Issues during parsing, cannot print");
      }
      {
        PhaseFiles files(status.folder+"/latex", "requirements.tex", *parsed.phases);
        generators::latex::printRequirements(*parsed.sections, *parsed.requirements, files);
      }
      {
        PhaseFiles files(status.folder+"/latex", "scenes.tex", *parsed.phases);
        generators::latex::printScenes(*parsed.sections, *parsed.scenes, files);
      }
      {
        std::string printFile = status.folder + "/latex/labels.tex";
        std::fstream file(printFile, std::fstream::out);
        if (!file) {
          throw Exception(Exception::Kind::User, "Could not open target file %1% for printing.", {printFile});
        }
        generators::latex::printLabels(storage->getNodeCollection(), *parsed.shortcuts, file);
      }
      status.messageFunction(Status::MessageKind::Message, "Generated requirements documents", {});
  
    }
    
    Print::Print() {}
    
    Print::Print(Parser& parser) {
      if(parser.getRemaining()!="") {
        throw Exception(Exception::Kind::User, "Print has no parameters.");
      }
    }
  }
}