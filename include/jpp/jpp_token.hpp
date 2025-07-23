#ifndef DTAMESV_JSON_PLUS_PLUS_TOKEN
#define DTAMESV_JSON_PLUS_PLUS_TOKEN

#include <cstddef>
#include <string_view>

namespace jpp{
  class Token{
    public:
      enum class Type{
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
        End,
        Invalid
      };

      std::size_t line;
      std::size_t column;
      Type type;
      std::string_view lexeme;

      Token( jpp::Token&& ) = default;
      jpp::Token& operator = ( const jpp::Token& ) = default;
      jpp::Token& operator = ( jpp::Token&& ) = default;

      Token( std::size_t, std::size_t, jpp::Token::Type, std::string_view );
      Token( std::size_t, std::size_t, jpp::Token::Type );

      const char* stringType();
  };
}

jpp::Token::Token( std::size_t line, std::size_t column, jpp::Token::Type type, std::string_view lexeme ) :
  line( line ),
  column( column ),
  type( type ),
  lexeme( lexeme ){
}

jpp::Token::Token( std::size_t line, std::size_t column, jpp::Token::Type type ) :
  line( line ),
  column( column ),
  type( type ){
}

const char* jpp::Token::stringType(){
  switch ( type ){
    case jpp::Token::Type::LeftBrace:
      return "LeftBrace";
    case jpp::Token::Type::RightBrace:
      return "RightBrace";
    case jpp::Token::Type::LeftBracket:
      return "LeftBracket";
    case jpp::Token::Type::RightBracket:
      return "RightBracket";
    case jpp::Token::Type::Number:
      return "Number";
    case jpp::Token::Type::String:
      return "String";
    case jpp::Token::Type::True:
      return "True";
    case jpp::Token::Type::False:
      return "False";
    case jpp::Token::Type::Null:
      return "Null";
    case jpp::Token::Type::Colon:
      return "Colon";
    case jpp::Token::Type::Comma:
      return "Comma";
    case jpp::Token::Type::End:
      return "End";
    case jpp::Token::Type::Invalid:
      return "Invalid";
    default: return "";
  }
}

#endif