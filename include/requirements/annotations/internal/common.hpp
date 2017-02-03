#pragma once

#include "requirements/id.hpp"
#include <map>
#include <list>

namespace requirements {
  namespace annotations {
    namespace internal {
      template<typename Entry>
      class CommonBuilder;
  
      template<typename Entry>
      class Common {
      protected:
        using EntryMap = std::map<::requirements::Id, Entry>;
        using RootEntryList = std::list<::requirements::Id>;
        EntryMap entries;
        RootEntryList rootEntries;
  
      public:
        typename EntryMap::const_iterator begin() { return entries.cbegin(); }
        typename EntryMap::const_iterator end() { return entries.cend(); }

        Entry& access(::requirements::Id id) { return entries.at(id); }

        const RootEntryList& getRootEntries() const { return rootEntries; }

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

        void insertRootEntry(::requirements::Id id) {
          rootEntries.emplace_back(id);
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