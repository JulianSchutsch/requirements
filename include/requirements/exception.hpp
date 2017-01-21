#pragma once

#include <exception>
#include <string>

namespace requirements {
    
    class Exception : public std::exception {
    private:
      std::string reason;
    public:
      Exception(const std::string &a_reason)
        : reason(a_reason) {}
      
      const char *what() const noexcept override { return reason.c_str(); }
    };
    
}