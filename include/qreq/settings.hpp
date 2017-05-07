#pragma once

#include <vector>
#include <list>
#include <unordered_map>

#include "qreq/f_code.hpp"

namespace qreq{

  class Settings{
    std::list<std::string> _last_projects;
    std::vector<std::string> _last_commands;
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
    const std::vector<std::string>& last_commands() const;
    void last_commands(std::vector<std::string> const& commands);

    //Key overrides
    std::unordered_map<std::string,F_code> key_overrides;
  protected:
    Settings() = default;
  };
}
