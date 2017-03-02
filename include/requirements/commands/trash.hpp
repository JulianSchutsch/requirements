#pragma once

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Trash : public ICommand {
    private:
      internal::Selection selection;
      ::requirements::Id trashId;
      bool hasTrashId = false;
    public:
      void execute(Status& status) override;
      Trash(::requirements::Id id, ::requirements::Id a_trashId);
      Trash(Parser& parser);
    };
  }
}