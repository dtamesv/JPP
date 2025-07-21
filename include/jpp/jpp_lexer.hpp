#ifndef DTAMESV_JSON_PLUS_PLUS_LEXER
#define DTAMESV_JSON_PLUS_PLUS_LEXER

#include <cstddef>
#include <string>
#include <string_view>

#include "jpp_token.hpp"

namespace jpp{
  class Lexer{
    public:
      Lexer( const std::string& );

      bool hasNext();
      jpp::Token next();

    private:
      const std::string& input_;
      std::size_t line_;
      std::size_t column_;
      std::size_t offset_;

      jpp::Token nextNumber();
      jpp::Token nextString();
      jpp::Token nextBoolean();
      jpp::Token nextNull();
  };
}

#include "jpp_lexer_def.hpp"

#endif