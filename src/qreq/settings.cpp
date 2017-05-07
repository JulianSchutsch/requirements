#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
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

void Settings::load(){
  _last_projects.clear();
  pt::ptree tree;
  try {
    pt::read_info(settings_filename(), tree);
  } catch(pt::info_parser_error& exc) {
    std::cout<<"Could not read config file: "<<exc.message()<<" : "<<exc.filename()<<std::endl;
  }

  //Load Project list
  for(auto &val:tree.get_child("qreq.projects")){
    _last_projects.push_back(val.second.data());
  }

  if(_last_projects.size()>0) current_project=_last_projects.back();

  //Load command list
  {
    auto commands = tree.get_child("qreq.commands", {});
    for (auto &val:commands) {
      _last_commands.push_back(val.second.data());
    }
  }

  {
    auto keycodes = tree.get_child("qreq.keycodes", {});
    for (auto &val:keycodes) {
      //val.second ist ein Baum
      pt::ptree keytree = val.second;
      F_code keydata;
      for (auto &va:keytree) {
        std::cout << va.first << " " << va.second.data() << std::endl;
        if (va.first == "keyname") keydata.key = va.second.data();
        else if (va.first == "caption") keydata.caption = va.second.data();
        else if (va.first == "command") keydata.command = va.second.data();
        else if (va.first == "params") keydata.params = (va.second.data() == "true" ? true : false);
      }
      key_overrides[keydata.key] = keydata;
    }
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
    keytree.add("keyname",overkey.second.key);
    keytree.add("caption",overkey.second.caption);
    keytree.add("command",overkey.second.command);
    keytree.add("params",overkey.second.params);
    tree.add_child("qreq.keycodes.override",keytree);
  }

  pt::write_info(settings_filename(), tree);
}

std::string Settings::settings_filename(){
  const char *homedir = getenv("HOME");
  if (homedir == NULL) {
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
  if(_last_projects.empty() || _last_projects.back()!=filename){
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
