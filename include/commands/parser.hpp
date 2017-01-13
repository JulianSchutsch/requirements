#pragma once

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
    TokenType tokenType = TokenType::Unknown;
    std::string tokenString;
  public:
    TokenType getTokenType() const { return tokenType; }
    const std::string& getTokenString() const { return tokenString; }
    Parser(const std::string& str);
    bool next();
  };
}