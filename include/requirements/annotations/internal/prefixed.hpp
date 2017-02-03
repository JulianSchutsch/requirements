#pragma once

#include <string>
#include <map>

#include "requirements/exception.hpp"
#include "requirements/annotations/internal/common.hpp"
#include "requirements/annotations/majorprefix.hpp"

namespace requirements {
  namespace annotations {
    namespace internal {
      template<typename BaseType>
      class PrefixedBuilderScope;
  
      template<typename BaseType>
      class PrefixedBuilder final {
        PrefixedBuilderScope<BaseType> *currentScope = nullptr;
        Common<BaseType>& collection;

        friend class PrefixedBuilderScope<BaseType>;
        MajorPrefix& majorPrefix;
  
      public:
        PrefixedBuilder(Common<BaseType> &a_collection, MajorPrefix& a_majorPrefix)
          : collection(a_collection)
          , majorPrefix(a_majorPrefix) {}
    
        void setMajorPrefix(const std::string &prefix) {
          if (currentScope != nullptr) {
            throw Exception(Exception::Kind::Internal, "Cannot change major prefix with current active scope");
          }
          majorPrefix.set(prefix);
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
          previousScope = builder.currentScope;
          builder.currentScope = this;
          if (previousScope != nullptr) {
            ++previousScope->number;
            prefix = previousScope->prefix + "." + std::to_string(previousScope->number);
          } else {
            prefix = builder.majorPrefix.consume();
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