#include "req/config.hpp"

#include <tinyxml.h>

#include "util/path.hpp"

namespace req {
 
  void Config::load_folder(TiXmlElement& element) {
    const char* e_folder = element.GetText();
    if(e_folder!=nullptr) {
      folder = e_folder;
    }
  }
  
  bool Config::load() {
    TiXmlDocument doc;
    if(!doc.LoadFile(util::getConfigPath()+".req_status.xml")) {
      return false;
    }
    TiXmlElement* element = doc.FirstChildElement();
    while(element) {
      auto value = element->Value();
      if(strcmp(value, "folder")==0) {
        load_folder(*element);
      }
      element = element->NextSiblingElement();
    }
    return true;
  }
  
  bool Config::save() {
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement* root = new TiXmlElement("config");
    doc.LinkEndChild(root);
    TiXmlElement* folder=new TiXmlElement("folder");
    root->LinkEndChild(folder);
    doc.SaveFile(util::getConfigPath()+".req_status.xml");
    return true;
  }
  
}