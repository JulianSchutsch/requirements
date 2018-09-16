#include "requirements/exception.hpp"
#include "requirements/status.hpp"
#include "requirements/commands/internal/selection.hpp"

#include "requirements/exception.hpp"

#include "requirements/istorage.hpp"
#include "requirements/select.hpp"

namespace requirements {
  namespace commands {
    namespace internal {
      
      Selection::Selection() {
        selectionKind = SelectionKind::ByIds;
      }
      
      Selection::Selection(Parser &parser) {
        selectionKind = SelectionKind::ByPatterns;
        while (parser.nextSimple()) {
          patterns.emplace_back(parser.getTokenString());
        }
      }
      
      Selection::Selection(::requirements::Id id) {
        selectionKind = SelectionKind::ByIds;
        ids = {id};
      }
      
      Selection::Selection(std::vector<::requirements::Id> id) {
        selectionKind = SelectionKind::ByIds;
        ids = id;
      }
      
      ::requirements::NodePtr Selection::extractNode(::requirements::IStorage &storage, Status &status) const {
        auto result = extractNodes(storage);
        if (result.size() == 0) {
          ::requirements::selectFromIds(storage.getNodeCollection(), status.selections[0], result);
        }
        if (result.size() != 1) {
          throw Exception(Exception::Kind::User, "Command requires exactly one node");
        }
        return result[0];
      }
      
      ::requirements::NodePtr Selection::extractNode(::requirements::IStorage &storage) const {
        auto result = extractNodes(storage);
        if (result.size() != 1) {
          throw Exception(Exception::Kind::User, "Command requires exactly one node");
        }
        return result[0];
      }
      
      std::vector<requirements::Id> Selection::extractIds(::requirements::IStorage &storage) const {
        if (selectionKind == SelectionKind::ByIds) {
          return std::move(ids);
        }
        auto selected = ::requirements::select(storage.getNodeCollection(), patterns);
        std::vector<::requirements::Id> result;
        result.reserve(selected.size());
        for (auto e:selected) {
          result.emplace_back(e->getId());
        }
        return result;
      }
      
      std::vector<::requirements::NodePtr> Selection::extractNodes(::requirements::IStorage &storage) const {
        if (selectionKind == SelectionKind::ByPatterns) {
          return ::requirements::select(storage.getNodeCollection(), patterns);
        }
        std::vector<::requirements::NodePtr> result;
        ::requirements::selectFromIds(storage.getNodeCollection(), ids, result);
        return result;
      }
    }
  }
}