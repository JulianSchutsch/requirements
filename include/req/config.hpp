#pragma once

#include <string>
#include <list>

class TiXmlElement;

namespace req {
  
  
  class Config {
  private:
    void load_folder(TiXmlElement& element);
  public:
    using Selection = std::list<std::string>;
    std::string folder;
    std::list<Selection> selections;
    bool load();
    bool save();
  };
  
}