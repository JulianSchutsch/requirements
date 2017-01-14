#include "commands/command_select.hpp"

#include "requirements/select.hpp"

#include "commands/status.hpp"
#include "commands/parser.hpp"

namespace commands {

  void Command_Select::execute(Status& status) {
    switch(selectionKind) {
      case SelectionKind::ByIds: {
        status.selections[0] = std::move(ids);
        break;
      }
      case SelectionKind::ByPatterns: {
        auto storage = status.openStorage();
        auto selected = ::requirements::select(storage->getNodeCollection(), patterns);
        status.selections[0].clear();
        status.selections[0].reserve(selected.size());
        for (auto e:selected) {
          status.selections[0].emplace_back(e->getId());
        }
        break;
      }
    }
  }

  Command_Select::Command_Select(std::vector<::requirements::Id> a_ids)
    : selectionKind(SelectionKind::ByIds)
    , ids(a_ids) {}

  Command_Select::Command_Select(Parser& parser)
    : selectionKind(SelectionKind::ByPatterns) {
    while(parser.nextSimple()) {
      patterns.emplace_back(parser.getTokenString());
    }
  }

}