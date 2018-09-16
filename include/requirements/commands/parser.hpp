#pragma once

#include <string>

namespace requirements {
  namespace commands {
    class Parser {
    public:
      enum class TokenType {
        Symbol,
        String,
        Integer,
        End,
        Unknown
      };
    private:
      std::string string;
      std::string::const_iterator pos;
      std::string::const_iterator tokenStart{};
      TokenType tokenType = TokenType::Unknown;
      
      void consumeSpace();
      bool nextSymbol();
      bool nextInteger();
    
    public:
      TokenType getTokenType() const { return tokenType; }
      
      std::string getTokenString() const { return std::string(tokenStart, pos); }
      
      std::string getRemaining() {
        consumeSpace();
        return std::string(pos, string.cend());
      }
      
      bool nextSimple();
      Parser(const std::string &str);
      bool next();
    };
  }
}