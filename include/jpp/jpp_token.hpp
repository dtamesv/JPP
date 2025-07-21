#ifndef DTAMESV_JSON_PLUS_PLUS_TOKEN
#define DTAMESV_JSON_PLUS_PLUS_TOKEN

#include <cstddef>
#include <string_view>

namespace jpp{
  class Token{
    public:
      enum Class Type{
        LeftBrace = 0,
        RightBrace,
        LeftBracket,
        RightBracket,
        Number,
        String,
        True,
        False,
        Null,
        Colon,
        Comma,
        EndOfFile,
        Invalid
      };

      std::size_t line;
      std::size_t column;
      Type type;
      std::string_view value;
  };
}

#endif