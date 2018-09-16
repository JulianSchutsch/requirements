#pragma once

#include <map>
#include <vector>
#include <list>
#include <unordered_map>

namespace qreq{

  class Settings{
  public:
    using Sequence = std::list<std::string>;
    using Sequences = std::map<std::string, Sequence>;
  private:
    std::list<std::string> _last_projects{};
    std::vector<std::string> _last_commands{};
    std::string settings_filename();
    Sequences _sequences{};
  public:

    Settings(Settings const&) = delete;             // Copy construct
    Settings(Settings&&) = delete;                  // Move construct
    Settings& operator=(Settings const&) = delete;  // Copy assign
    Settings& operator=(Settings &&) = delete;      // Move assign

    void load();
    void store();
    void add_project(std::string const& filename);

    const std::list<std::string>& last_projects() const;

    //The current selected project
    std::string current_project{};
    const std::vector<std::string>& last_commands() const;
    void last_commands(std::vector<std::string> const& commands);
    const Sequences& sequences() const;
    void setSequences(Sequences& sequences);

    Settings() = default;
  };
}
