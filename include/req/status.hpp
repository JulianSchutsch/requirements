#pragma once

#include <string>
#include <list>

class TiXmlElement;

namespace req {
  
  
  class Status {
  private:
    void load_folder(TiXmlElement& element);
    void load_status(TiXmlElement& root);
    void save_folder(TiXmlElement& root);
  public:
    using Selection = std::list<std::string>;
    std::string folder;
    std::list<Selection> selections;
    bool load();
    bool save();
  };
  
}