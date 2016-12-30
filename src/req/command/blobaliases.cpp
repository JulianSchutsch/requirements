#include "req/command/blobaliases.hpp"

#include <boost/algorithm/string/join.hpp>

#include "requirements/storage/text.hpp"
#include "requirements/blob.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_blobAliases(Status& status, const std::vector<std::string>& parameters) {
      ::requirements::storage::Text storage(status.folder, true);
      if(parameters.size()<1) {
        throw Exception("BlobAliases requires at least one parameter to uniquely identify a blob");
      }
      auto selected = ::requirements::selectBlobs(storage, {parameters[0]});
      if(selected.size()!=1) {
        throw Exception("BlobAliases requires exactly one blob to be selected");
      }
      auto itNext = std::begin(parameters);
      ++itNext;
      auto slice = std::vector<std::string>(itNext, std::end(parameters));
      auto alias = boost::algorithm::join(slice, " ");
      storage.setBlobAliases(selected[0], alias);
    }
  }
}