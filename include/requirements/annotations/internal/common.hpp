#pragma once

#include "requirements/id.hpp"
#include <map>

namespace requirements {
  namespace annotations {
    namespace internal {
      template<typename Entry>
      class CommonBuilder;
  
      template<typename Entry>
      class Common {
      protected:
        std::map<::requirements::Id, Entry> entries;
  
      public:
        bool has(::requirements::Id id) const {
          return entries.find(id) != entries.end();
        }
    
        const Entry &get(::requirements::Id id) const {
          return entries.at(id);
        }
        
        void insert(::requirements::Id id, Entry&& entry) {
          entries.emplace(id, std::move(entry));
        }
        void insert(::requirements::Id id, const Entry& entry) {
          entries.emplace(id, entry);
        }
      };
  
      template<typename Entry>
      class CommonBuilder {
      private:
        Common<Entry> &common;
      public:
        CommonBuilder(Common<Entry> &a_common)
          : common(a_common) {}
    
        void set(::requirements::Id id, Entry&& entry) {
          common.insert(id, std::move(entry));
        }
        
        void set(::requirements::Id id, const Entry& entry) {
          common.insert(id, entry);
        }
      };
    }
  }
}