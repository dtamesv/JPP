jpp::Lexer::Lexer( const std::string& input ) :
  input( input ),
  line( 1 ),
  column( 1 ),
  offset( 0 ){
}

bool jpp::Lexer::hasNext(){
  return offset < input.length();
}


jpp::Token jpp::Lexer::next(){
  // Ignore whitespaces
  while (
    input[ offset ] == ' '
    || input[ offset ] == '\n'
    || input[ offset ] == '\r'
    || input[ offset ] == '\t'
  ){
    if ( input[ offset++ ] == '\n' ){
      ++line;
      column = 1;
    }
  }

  if ( !hasNext() ) return { line, column, jpp::Token::Type::End };

  // Read next token
  switch ( input[ offset ] ){
    case '{':
      ++offset;
      return { line, column++, jpp::Token::Type::LeftBrace };

    case '}':
      ++offset;
      return { line, column++, jpp::Token::Type::RightBrace };

    case '[':
      ++offset;
      return { line, column++, jpp::Token::Type::LeftBracket };

    case ']':
      ++offset;
      return { line, column++, jpp::Token::Type::RightBracket };
    
    case ':':
      ++offset;
      return { line, column++, jpp::Token::Type::Colon };
    
    case ',':
      ++offset;
      return { line, column++, jpp::Token::Type::Comma };

    case '-':
    [[fallthrough]];
    case '0':
    [[fallthrough]];
    case '1':
    [[fallthrough]];
    case '2':
    [[fallthrough]];
    case '3':
    [[fallthrough]];
    case '4':
    [[fallthrough]];
    case '5':
    [[fallthrough]];
    case '9':
      return nextNumber();

    case '"':
      return nextString();

    case 't':
    [[fallthrough]];
    case 'f':
      return nextBoolean();

    case 'n':
      return nextNull();

    default:
      ++offset;
      return { line, column++, jpp::Token::Type::Invalid };
  }
}

jpp::Token jpp::Lexer::nextNumber(){
  return { line, column++, jpp::Token::Type::Number };
}

jpp::Token jpp::Lexer::nextString(){
  return { line, column++, jpp::Token::Type::String };
}

jpp::Token jpp::Lexer::nextBoolean(){
  std::size_t col( column );

  if ( std::strncmp( "true", &input[ offset ], 4 ) == 0 ){
    offset += 4;
    column += 4;

    return { line, col, jpp::Token::Type::True };
  }
  
  if ( std::strncmp( "false", &input[ offset ], 5 ) == 0 ){
    offset += 5;
    column += 5;
    return { line, col, jpp::Token::Type::False };
  }

  return nextInvalid();
}

jpp::Token jpp::Lexer::nextNull(){
  if ( std::strncmp( "null", &input[ offset ], 4 ) == 0 ){
    std::size_t col( column );
    offset += 4;
    column += 4;
    return { line, col, jpp::Token::Type::Null };
  }

  return nextInvalid();
}

jpp::Token jpp::Lexer::nextInvalid(){
  return nextInvalid( offset );
}

jpp::Token jpp::Lexer::nextInvalid( std::size_t ini ){
  while (
    hasNext()
    && input[ offset ] != '\n'
    && input[ offset ] != '\r'
    && input[ offset ] != ' '
    && input[ offset ] != ','
    && input[ offset ] != ':'
  ){
    ++offset;
    ++column;
  };

  return { line, column, jpp::Token::Type::Invalid, std::string_view( &input[ ini ], offset - ini ) };
}