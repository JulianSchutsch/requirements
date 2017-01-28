#pragma once

#include <string>
#include <map>

#include "requirements/exception.hpp"
#include "requirements/annotations/internal/common.hpp"

namespace requirements {
  namespace annotations {
    namespace internal {
      template<typename BaseType>
      class PrefixedBuilderScope;
  
      template<typename BaseType>
      class PrefixedBuilder final {
        std::map<std::string, int> majorPrefixStates;
        PrefixedBuilderScope<BaseType> *currentScope = nullptr;
        Common<BaseType>& collection;
        std::string majorPrefix;
        int majorNumber = 0;
    
        friend class PrefixedBuilderScope<BaseType>;
  
      public:
        PrefixedBuilder(Common<BaseType> &a_collection)
          : collection(a_collection) {}
    
        void setMajorPrefix(const std::string &prefix) {
          if (currentScope != nullptr) {
            throw Exception(Exception::Kind::Internal, "Cannot change major prefix with current active scope");
          }
          majorPrefixStates[majorPrefix] = majorNumber;
          majorPrefix = prefix;
          {
            auto continuationIt = majorPrefixStates.find(prefix);
            majorNumber = (continuationIt != majorPrefixStates.end()) ? continuationIt->second : 0;
          }
        }
    
        PrefixedBuilder(const PrefixedBuilder &) = delete;
        PrefixedBuilder &operator=(const PrefixedBuilder &) = delete;
      };
  
      template<typename BaseType>
      class PrefixedBuilderScope final {
      private:
        int number = 0;
        std::string prefix;
        PrefixedBuilderScope<BaseType> *previousScope;
        PrefixedBuilder<BaseType> &builder;
      public:
        template<typename... BaseTypeParams>
        PrefixedBuilderScope(PrefixedBuilder<BaseType> &a_builder, ::requirements::Id nodeId, BaseTypeParams &&... params)
          : builder(a_builder) {
          if (builder.majorPrefix.empty()) {
            throw Exception(Exception::Kind::Internal, "Require major prefix");
          }
          previousScope = builder.currentScope;
          builder.currentScope = this;
          if (previousScope != nullptr) {
            ++previousScope->number;
            prefix = previousScope->prefix + "." + std::to_string(previousScope->number);
          } else {
            ++builder.majorNumber;
            prefix = builder.majorPrefix + std::to_string(builder.majorNumber);
          }
          builder.collection.insert(nodeId, BaseType(prefix, std::forward<BaseTypeParams>(params)...));
        }
    
        ~PrefixedBuilderScope() {
          builder.currentScope = previousScope;
        }
    
        PrefixedBuilderScope(const PrefixedBuilderScope &) = delete;
    
        PrefixedBuilderScope &operator=(const PrefixedBuilderScope &) = delete;
    
        const std::string &getKey() const { return prefix; }
      };
    }
  }
}