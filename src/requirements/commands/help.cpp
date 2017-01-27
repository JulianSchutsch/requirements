#include "requirements/commands/help.hpp"

#include <map>
#include <algorithm>

#include "requirements/status.hpp"
#include "requirements/exception.hpp"
#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
  
    static const std::map<std::string, std::vector<const char*>> helpTexts = {
      {"edit",
       {"The Edit command calls the editor for a single currently selected or specified node."}},
      {"new",
       {"The New command creates a new element and selects it."}},
      {"delete",
       {"The Delete command deletes a single specified element. It is not possible to delete an element by selection."}},
      {"folder",
       {"The folder command selects a root folder for the requirements storage.",
        "It is adviseable to have this folder under version control (e.g. git)."}},
      {"select",
        {"Select selects a set of nodes by pattern matching. Therefore you can select any number of nodes using regular expressions."}},
      {"newblob",
        {"NewBlob adds a blob. You must specify a source file to import."}},
      {"listblobs",
        {"ListBlobs lists all available blobs with aliases"}},
      {"blobaliases",
        {"BlobAliases assigns aliases to a blob. The first parameter selects a blob (safest is the uuid), the remaining parameters are attached as aliases."}},
      {"showfolder",
        {"ShowFolder simply outputs the currently selected folder"}},
      {"nextto",
        {"NextTo assigns the currently selected node as next brother to the specified one."}},
      {"previousto",
        {"PreviousTo assigns the currently selected node as the previous brother to the specified one."}},
      {"lastof",
        {"LastOf assigns the currently selected node as the last child of the specified one."}},
      {"firstof",
        {"FirstOf assigns the currently selected node as the first child of the specified one."}}
    };
    
    void Help::execute(Status& status) {
      if(specificCommand!="") {
        auto it = helpTexts.find(specificCommand);
        for(auto& e: it->second) {
          status.messageFunction(Status::MessageKind::Content, e, {});
        }
      } else {
        status.messageFunction(Status::MessageKind::Content, "*** Help for requirements ***", {});
        status.messageFunction(Status::MessageKind::Content, "Available (documented) commands:", {});
        for(auto& e:helpTexts) {
          status.messageFunction(Status::MessageKind::Content, e.first, {});
        }
      }
    }
  
    Help::Help(Parser& parser) {
      if(parser.nextSimple()) {
        specificCommand = parser.getTokenString();
        std::transform(specificCommand.begin(), specificCommand.end(), specificCommand.begin(), ::tolower);
        auto it = helpTexts.find(specificCommand);
        if(it==helpTexts.end()) {
          throw Exception(Exception::Kind::User, "No help available for %s.", {specificCommand});
        }
        if(parser.nextSimple()) {
          throw requirements::Exception(Exception::Kind::User, "Help can only take on parameter.", {});
        }
      }
    }
  }
}