#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "greq/settings.hpp"

namespace pt = boost::property_tree;

namespace greq{

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

    for(auto &val:tree.get_child("greq.projects")){

      std::cout << "Push: " << val.second.data() << std::endl;
      _last_projects.push_back(val.second.data());
    }
  }
  catch(std::exception& e){
    //Nothing read, data stay with default values
  }
}

void Settings::store(){
  pt::ptree tree;

  for(auto &name:_last_projects){
    tree.add("greq.projects.project", name);
  }

  pt::write_info(settings_filename(), tree);
}

std::string Settings::settings_filename(){
  const char *homedir;
  if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
  }
  return (std::string)homedir+"/.greqrc";
}

///
/// Adds a project file to the list of last projects, if it does not already exists.
/// If the length of the list exceeds some value, the first value will be popped.
///
void Settings::add_project(std::string const& filename){
  //look for existing name
  bool found=false;
  for(auto &name:_last_projects){
    if(name==filename) found=true;
  }
  if(found==false){
    //we can add it
    //look for length
    if(_last_projects.size()>10){
      //remove oldest (==first) element
      _last_projects.pop_front();
    }
    _last_projects.push_back(filename);
  }
}

const std::list<std::string>& Settings::last_projects() const{
  return _last_projects;
}

}
