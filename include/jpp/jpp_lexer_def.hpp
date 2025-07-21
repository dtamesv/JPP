#include <cstddef>
#include <string>
#include <string_view>

jpp::Lexer::Lexer( const std::string& input ) :
  input_( input ),
  line_( 0 ),
  column_( 0 ),
  offset_( 0 ){
}

bool jpp::Lexer::hasNext(){
  return offset_ < input_.length();
}


jpp::Token jpp::Lexer::next(){
  if ( !hasNext() ) return { line_, column_, jpp::Token::Type::End };

  // Ignore whitespaces
  for ( bool ws( true ); ws; ++offset_ ){
    switch ( input_[ offset_ ] ){
      case '\n':
        ++line_;
      break;
      case ' ':
      [[fallthrough]]
      case '\t':
        ++column_;
      break;
      case '\r': break; // Ignore \r
      default:
        ws = false;
    }
  }

  // Read next token
  switch ( input_[ offset_ ] ){
    case '{':
      ++offset_;
      return { line_, column_++, jpp::Token::Type::LeftBrace };

    case '}':
      ++offset_;
      return { line_, column_++, jpp::Token::Type::RightBrace };

    case '[':
      ++offset_;
      return { line_, column_++, jpp::Token::Type::LeftBracket };

    case ']':
      ++offset_;
      return { line_, column_++, jpp::Token::Type::RightBracket };

    case '-':
    [[fallthrough]]
    case '0':
    [[fallthrough]]
    case '1':
    [[fallthrough]]
    case '2':
    [[fallthrough]]
    case '3':
    [[fallthrough]]
    case '4':
    [[fallthrough]]
    case '5':
    [[fallthrough]]
    case '9':
      return nextNumber();

    case '"':
      return nextString();

    case 't':
    [[fallthrough]]
    case 'f':
      return nextBoolean();

    case 'n':
      return nextNull();

    default:
      ++offset_;
      return { _line, _column++, jpp::Token::Type::Invalid };
  }
}

jpp::Token jpp::Lexer::nextNumber(){

}

jpp::Token jpp::Lexer::nextString(){

}

jpp::Token jpp::Lexer::nextBoolean(){

}

jpp::Token jpp::Lexer::nextNull(){

}