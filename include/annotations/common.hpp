#pragma once

#include "requirements/id.hpp"
#include <map>

namespace annotations {
  template<typename Entry>
  class CommonBuilder;
  
  template<typename Entry>
  class Common {
  protected:
    std::map<::requirements::Id, Entry> entries;
    friend class CommonBuilder<Entry>;
  public:
    bool has(::requirements::Id id) const {
      return entries.find(id)!=entries.end();
    }
    const Entry& get(::requirements::Id id) const {
      return entries.at(id);
    }
  };
  
  template<typename Entry>
  class CommonBuilder {
  private:
    Common<Entry>& common;
  public:
    CommonBuilder(Common<Entry>& a_common)
      : common(a_common) {}
    void set(::requirements::Id id, const Entry& entry) {
      common.entries.emplace(id, entry);
    }
  };
}