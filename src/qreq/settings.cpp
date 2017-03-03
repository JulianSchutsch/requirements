#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <array>

#include "qreq/settings.hpp"

namespace pt = boost::property_tree;

namespace qreq{

Settings& Settings::getInstance(){
  // Since it's a static variable, if the class has already been created,
  // It won't be created again.
  // And it **is** thread-safe in C++11.

  static Settings myInstance;

  // Return a reference to our instance.
  return myInstance;
}

Settings::Settings() :
  _last_projects(std::list<std::string>())
{
}

Settings::~Settings(){
}

void Settings::load(){
  _last_projects.clear();
  pt::ptree tree;
  try{
    pt::read_info(settings_filename(), tree);

    //Load Project list
    for(auto &val:tree.get_child("qreq.projects")){

      //std::cout << "Push: " << val.second.data() << std::endl;
      _last_projects.push_back(val.second.data());
    }
    //TODO: Das muss konfigurierbar sein
    if(_last_projects.size()>0) current_project=_last_projects.back();
    //Load command list
    for(auto &val:tree.get_child("qreq.commands")){

      //std::cout << "Push: " << val.second.data() << std::endl;
      _last_commands.push_back(val.second.data());
    }
    //Keycodes
    //Das ist ja eine blöde Möglichkeit durch den Tree zu eiern. Gehts vielleicht auch besser?
    std::array<std::string,10> keycodes={"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10"};
    for(auto &keyname:keycodes){
      F_code keydata;
      auto child=tree.get_child("qreq.keycodes."+keyname);
      if(child.size()>0){
        for(auto &val:tree.get_child("qreq.keycodes."+keyname)){
          if(val.first=="caption") keydata.caption=val.second.data();
          if(val.first=="command") keydata.command=val.second.data();
          if(val.first=="params") keydata.params=(val.second.data()=="true"?true:false);
          std::cout << val.first << "   " << val.second.data() << std::endl;
        }
        key_overrides[keyname]=keydata;
      }
    }
  }
  catch(std::exception& e){
    //Nothing read, data stay with default values
  }
}

void Settings::store(){
  pt::ptree tree;

  for(auto &name:_last_projects){
    tree.add("qreq.projects.project", name);
  }
  for(auto &name:_last_commands){
    tree.add("qreq.commands.command", name);
  }
  //Speichern der Key-Overrides
  for(auto& overkey:key_overrides){
    pt::ptree keytree;
    keytree.add("caption",overkey.second.caption);
    keytree.add("command",overkey.second.command);
    keytree.add("params",overkey.second.params);
    tree.add_child("qreq.keycodes."+overkey.first,keytree);
  }

/*
  //Test: Speichern von structs
  pt::ptree f1tree;

  //f1tree.add("keyname","F1");
  f1tree.add("caption","F1 commit");
  f1tree.add("command","git commit -a -m");
  f1tree.add("params",true);

  pt::ptree f2tree;
  //f2tree.add("keyname","F2");
  f2tree.add("caption","F2 push");
  f2tree.add("command","git push");
  f2tree.add("params",false);

  tree.add_child("qreq.keycodes.F1",f1tree);
  tree.add_child("qreq.keycodes.F2",f2tree);
*/
  pt::write_info(settings_filename(), tree);
}

std::string Settings::settings_filename(){
  const char *homedir;
  if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
  }
  return (std::string)homedir+"/.qreqrc";
}

///
/// Adds a project file to the list of last projects, if it does not already exists.
/// If the length of the list exceeds some value, the first value will be popped.
///
void Settings::add_project(std::string const& filename){
  //look for existing name
  //Shortcut: See, if the new project is already the newest
  //if so, do nothing
  if(_last_projects.back()!=filename){
    std::list<std::string>::iterator it=_last_projects.begin();
    while(it!=_last_projects.end()){
      if(*it==filename){
        //Oh, schon drin. Jetzt aber raus damit
        it=_last_projects.erase(it);
      }
      else ++it;
    }
    if(_last_projects.size()>10){
      //remove oldest (==first) element
      _last_projects.pop_front();
    }
    _last_projects.push_back(filename);
  }
  current_project=filename;
}

const std::list<std::string>& Settings::last_projects() const{
  return _last_projects;
}

const std::vector<std::string>& Settings::last_commands() const{
  return _last_commands;
}

void Settings::last_commands(std::vector<std::string> const& commands){
  _last_commands=commands;
}



}
