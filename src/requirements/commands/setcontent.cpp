#include "requirements/commands/setcontent.hpp"

#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    void SetContent::execute(Status& status) {
      auto storage = status.openStorage();
      auto node = selection.extractNode(*storage, status);
      node->updateContent(content);
    }

    SetContent::SetContent(::requirements::Id id, const std::string& a_content)
      : selection(id)
      , content(a_content) {}

    SetContent::SetContent(Parser& parser)
      : content(parser.getRemaining()) {}

  }
}