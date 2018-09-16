#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"
#include "requirements/icommand.hpp"

class TiXmlElement;

namespace requirements {
  
  class Status {
  private:
    void load_folder(TiXmlElement& element);
    void load_selections(TiXmlElement& element);
    void load_status(TiXmlElement& root);
    void save_folder(TiXmlElement& root);
    void save_selections(TiXmlElement& root);
  public:
    enum MessageKind {
      InternalError,
      UserError,
      OtherError,
      Message,
      Content
    };
    using MessageFunction = std::function<void(MessageKind, const std::string&, const std::vector<std::string>& parameters)>;
    using EditFunction = std::function<void(NodePtr)>;
    MessageFunction messageFunction{};
    EditFunction editFunction{};
    std::unique_ptr<::requirements::IStorage> openStorage();
    using Selection = std::vector<requirements::Id>;
    std::string folder{};
    std::map<int, Selection> selections{};
    bool load(const std::string& filename="");
    bool save(const std::string& filename="");
    std::unique_ptr<Status> clone();
  };
  
}