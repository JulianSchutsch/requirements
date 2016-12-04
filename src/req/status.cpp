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

  void Status::load_selections(TiXmlElement& element) {
    int nr;
    if(!element.QueryIntAttribute("nr", &nr)) {
      return;
    }
    auto& mapEntry = selections[nr];
    TiXmlElement* selection = element.FirstChildElement();
    while(selection) {
      auto value = selection->Value();
      if(strcmp(value, "node")) {
        std::string uuid;
        if(selection->QueryStringAttribute("uuid", &uuid)) {
          mapEntry.push_back(uuid);
        }
      }
      selection = selection->NextSiblingElement();
    }
  }

  void Status::load_status(TiXmlElement& root) {
    TiXmlElement* element = root.FirstChildElement();
    while(element) {
      auto value = element->Value();
      if(strcmp(value, "folder")==0) {
        load_folder(*element);
      }
      if(strcmp(value, "selections")==0) {
        load_selections(*element);
      }
      element = element->NextSiblingElement();
    }
  }
  
  bool Status::load() {
    folder = "";
    selections.clear();
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
  
  void Status::save_selections(TiXmlElement& root) {
    for(auto& selection: selections) {
      TiXmlElement* e_selection=new TiXmlElement("selection");
      e_selection->SetAttribute("nr", std::to_string(selection.first));
      root.LinkEndChild(e_selection);
      for(auto& element:selection.second) {
        TiXmlElement* e_element=new TiXmlElement("node");
        e_element->SetAttribute("uuid", element);
        e_selection->LinkEndChild(e_element);
      }
    }
  }
  
  bool Status::save() {
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement* root = new TiXmlElement("status");
    doc.LinkEndChild(root);
    save_folder(*root);
    save_selections(*root);
    doc.SaveFile(util::getConfigPath()+".req_status.xml");
    return true;
  }
  
}