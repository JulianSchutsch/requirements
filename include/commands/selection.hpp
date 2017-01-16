#pragma once

#include <vector>

#include "requirements/node.hpp"

#include "commands/parser.hpp"

namespace requirements {
  class IStorage;
}

namespace commands {
  
  class Status;
  
  class Selection {
  private:
    enum class SelectionKind {
      ByIds,
      ByPatterns
    };
    SelectionKind selectionKind;
    std::vector<::requirements::Id> ids;
    std::vector<std::string> patterns;
  public:
    Selection(Parser& parser);
    Selection(::requirements::Id id);
    Selection(std::vector<::requirements::Id> id);
    std::vector<::requirements::Id> extractIds(::requirements::IStorage& storage) const;
    std::vector<::requirements::NodePtr> extractNodes(::requirements::IStorage& storage) const;
    ::requirements::NodePtr extractNode(::requirements::IStorage& storage, Status& status) const;
    ::requirements::NodePtr extractNode(::requirements::IStorage& storage) const;
  };
  
}