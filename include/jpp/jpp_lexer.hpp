#ifndef DTAMESV_JSON_PLUS_PLUS_LEXER
#define DTAMESV_JSON_PLUS_PLUS_LEXER

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>

#include "jpp_token.hpp"

namespace jpp{
  class Lexer{
    public:
      Lexer( const std::string& );

      bool eof();
      jpp::Token next();

    private:
      const std::string& input;
      std::size_t line;
      std::size_t column;
      std::size_t offset;

      char currChar();
      char nextChar();

      void ignoreWhitespaces();

      jpp::Token nextNumber();
      jpp::Token nextString();
      jpp::Token nextBoolean();
      jpp::Token nextNull();
      jpp::Token nextInvalid();

      bool isWhitespace();
      bool isWhitespace( char c );
      bool isDigit();
      bool isDigit( char c );
      bool isSymbol();
      bool isSymbol( char c );
  };
}

#include "jpp_lexer_def.hpp"

#endif