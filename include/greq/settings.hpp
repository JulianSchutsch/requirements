#pragma once

#include <vector>
#include <list>

namespace greq{
  class Settings{
    std::list<std::string> _last_projects;
    std::string settings_filename();
  public:
    static Settings& getInstance();

    Settings(Settings const&) = delete;             // Copy construct
    Settings(Settings&&) = delete;                  // Move construct
    Settings& operator=(Settings const&) = delete;  // Copy assign
    Settings& operator=(Settings &&) = delete;      // Move assign

    void load();
    void store();
    void add_project(std::string const& filename);

    const std::list<std::string>& last_projects() const;

    //The current selected project
    std::string current_project;
  protected:
    Settings();
    ~Settings();
  };
}
