#include "req/status.hpp"

#include <tinyxml.h>

#include "util/path.hpp"

namespace req {
 
  void Status::load_folder(TiXmlElement& element) {
    const char* e_folder = element.GetText();
    if(e_folder!=nullptr) {
      folder = e_folder;
    }
  }
  
  void Status::load_status(TiXmlElement& root) {
    TiXmlElement* element = root.FirstChildElement();
    while(element) {
      auto value = element->Value();
      if(strcmp(value, "folder")==0) {
        load_folder(*element);
      }
      element = element->NextSiblingElement();
    }
  }
  
  bool Status::load() {
    TiXmlDocument doc;
    if(!doc.LoadFile(util::getConfigPath()+".req_status.xml")) {
      return false;
    }
    TiXmlElement* element = doc.FirstChildElement();
    while(element) {
      auto value = element->Value();
      if(strcmp(value, "status")==0) {
        load_status(*element);
      }
      element = element->NextSiblingElement();
    }
    return true;
  }
  
  void Status::save_folder(TiXmlElement& root) {
    TiXmlElement* e_folder=new TiXmlElement("folder");
    TiXmlText* e_text=new TiXmlText(folder.c_str());
    e_folder->LinkEndChild(e_text);
    root.LinkEndChild(e_folder);
  }
  
  bool Status::save() {
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement* root = new TiXmlElement("status");
    doc.LinkEndChild(root);
    save_folder(*root);
    doc.SaveFile(util::getConfigPath()+".req_status.xml");
    return true;
  }
  
}