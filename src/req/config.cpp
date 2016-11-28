#include "req/config.hpp"

#include <tinyxml.h>

#include "util/path.hpp"

namespace req {
  
  bool Config::load() {
    TiXmlDocument doc(util::getConfigPath()+"req_status.xml");
  }
  
  bool Config::save() {
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement* root = new TiXmlElement("config");
    doc.LinkEndChild(root);
    TiXmlElement* folder=new TiXmlElement("folder");
    root->LinkEndChild(folder);
    doc.SaveFile(util::getConfigPath()+"req_status.xml");
  }
  
}