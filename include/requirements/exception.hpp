#pragma once

#include <exception>
#include <string>
#include <vector>
#include "util/formatstring.hpp"

namespace requirements {
    
    class Exception : public std::exception {
    public:
      enum class Kind {
        Internal,
        User,
        Other
      };
    private:
      Kind kind;
      std::string reason;
      std::vector<std::string> parameters {};
    public:
      Kind getKind() const { return kind; }
      const std::string& getReason() const { return reason; }
      const std::vector<std::string>& getParameters() const { return parameters; }
      Exception(Kind a_kind, const std::string& a_reason)
        : kind(a_kind)
        , reason(a_reason) {}
      Exception(Kind a_kind, const std::string& a_reason, const std::vector<std::string>& a_parameters)
        : kind(a_kind)
        , reason(a_reason)
        , parameters(a_parameters) {}
      
      const char *what() const noexcept override { return reason.c_str(); }
    };
    
}