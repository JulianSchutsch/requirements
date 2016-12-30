#include "req/select.hpp"

#include "requirements/istorage.hpp"
#include "requirements/select.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  std::vector<::requirements::NodePtr> selectNodes(Status& status, ::requirements::IStorage& storage, int index, const std::vector<std::string>& parameters) {
    auto& collection = storage.getNodeCollection();
    std::vector<requirements::NodePtr> selections;
    if(parameters.size()!=0) {
      selections = requirements::select(collection, parameters);
    } else {
      if(!requirements::selectFromIds(collection, status.selections[index], selections)) {
        throw Exception("Selection in status is not correct.");
      }
    }
    return std::move(selections);
  }
}