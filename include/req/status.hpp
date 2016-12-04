#pragma once

#include <string>
#include <vector>
#include <map>

class TiXmlElement;

namespace req {
  
  
  class Status {
  private:
    void load_folder(TiXmlElement& element);
    void load_selections(TiXmlElement& element);
    void load_status(TiXmlElement& root);
    void save_folder(TiXmlElement& root);
    void save_selections(TiXmlElement& root);
  public:
    using Selection = std::vector<std::string>;
    std::string folder;
    std::map<int, Selection> selections;
    bool load();
    bool save();
  };
  
}