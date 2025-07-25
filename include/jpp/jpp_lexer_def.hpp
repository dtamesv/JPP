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
  while ( isWhitespace() || input[ offset ] == '\r' ){
    if ( input[ offset++ ] == '\n' ){
      ++line;
      column = 1;
    }else if ( input[ offset ] != '\r' ){
      ++column;
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

char jpp::Lexer::currChar(){
  if ( offset < input.length )
    return input[ offset ];
  return '\0';
}

char jpp::Lexer::nextChar(){
  if ( offset < input.length )
    ++offset;
  if ( offset < input.length ){
    char c = input[ offset ];
    if ( c == '\n' ){
      ++line;
      column = 0;
    }else if ( c != '\r' )
      ++column;
    return c;
  }
  return '\0';
}

jpp::Token jpp::Lexer::nextNumber(){
  enum class FAState{
    Ini = 0,
    Sign,
    IntDigit,
    IntZero,
    DecDigit,
    Exp,
    ExpSign,
    ExpDigit,
    Valid,
    Invalid
  };

  std::size_t iniOffset = offset;
  std::size_t iniLine = line;
  std::size_t iniColumn = column;

  char c( currChar() );
  
  FAState state( FAState::Ini );

  do{
    switch ( state ){
      case FAState::Ini:
        if ( c == '-' ) state = FAState::Sign;
        else if ( c == '0' ) state = FAState::IntZero;
        else state = FAState::IntDigit;
      break;
      case FAState::Sign:
        if ( c == '0' ) state = FAState::IntZero;
        else if ( isDigit() ) state = FAState::IntDigit;
        else state = FAState::Invalid;
      break;
      case FAState::IntDigit:
        else if ( c == '.' ) state = FAState::DecDigit;
        else if ( c == 'e' || c == 'E' ) state = FAState::Exp;
        else if ( !isDigit() ) state = FAState::Invalid
      break;
      case FAState::IntZero:
        if ( c == '.' ) state = FAState::DecDigit;
        else if ( c == 'e' || c == 'E' ) state = FAState::Exp;
        else state = FAState::Invalid;
      break;
      case FAState::DecDigit:
        if ( c == 'e' || c == 'E' ) state = FAState::Exp;
        else if ( isSymbol() || isWhitespace() ) state = FAState::Valid;
        else if ( !isDigit() ) state = FAState::Invalid;
      break;
      case FAState::Exp:
        if ( c == '+' || c == '-' ) state = FAState::ExpSign;
        else if ( isDigit() ) state = FAState::ExpDigit;
        else state = FAState::Invalid;
      break;
      case FAState::ExpSign:
        if ( isDigit() ) state = FAState::ExpDigit;
        else state = FAState::Invalid;
      break;
      case FAState::ExpDigit:
        if ( isSymbol() || isWhitespace() ) state = FAState::Valid;
        else if ( !isDigit() ) state = FAState::Invalid;
      break;
      case FAState::Invalid:
        return { line, column, jpp::Token::Type::Invalid, { &input[ offset ], 1 } }
      break;
    }

    c = nextChar();
  }while ( state != FAState::Valid );

  return { iniLine, iniColumn, jpp::Token::Type::Number, { &input[ iniOffset ], 1 } };
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
    && !isWhitespace( input[ offset ] )
    && !isSymbol( input[ offset ] )
  ){
    ++offset;
    ++column;
  };

  return { line, column, jpp::Token::Type::Invalid, std::string_view( &input[ ini ], offset - ini ) };
}

bool jpp::Lexer::isWhitespace(){
  return isWhitespace( input[ offset ] );
}

bool jpp::Lexer::isWhitespace( char c ){
  return
    c == ' '
    || c == '\n'
    || c == '\t';
}

bool jpp::Lexer::isDigit(){
  return isDigit( input[ offset ] );
}

bool jpp::Lexer::isDigit( char c ){
  return
    c == '0'
    || c == '1'
    || c == '2'
    || c == '3'
    || c == '4'
    || c == '5'
    || c == '6'
    || c == '7'
    || c == '8'
    || c == '9';
}

bool jpp::Lexer::isSymbol(){
  return isSymbol( input[ offset ] );
}

bool jpp::Lexer::isSymbol( char c ){
  return
    c == '{'
    || c == '}'
    || c == '['
    || c == ']'
    || c == ':'
    || c == ',';
}