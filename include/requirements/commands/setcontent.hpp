#pragma once

#include <string>

#include <requirements/icommand.hpp>
#include <requirements/commands/internal/selection.hpp>

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class SetContent : public ICommand {
    private:
      internal::Selection selection{};
      std::string content{};
    public:
      void execute(Status& status) override;
      SetContent(::requirements::Id id, const std::string& a_content);
      SetContent(Parser& parser);
    };
  }
}