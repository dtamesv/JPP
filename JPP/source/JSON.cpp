#include "JSON.hpp"

jpp::JSON::JSON() :
    type( Type::Null ),
    value( "null" ),
    children( 0 ),
    parent( 0 ),
    previous( 0 ),
    next( 0 ){
}

jpp::JSON::JSON( const std::string &json ) :
    JSON(){
    parse( json );
}

jpp::JSON::JSON( const jpp::JSON &other ) :
    JSON(){
    this->operator = ( other );
}

jpp::JSON::~JSON(){
    setNull();
    key.clear();
    parent = 0;
    previous = 0;
    next = 0;
}

jpp::JSONIterator jpp::JSON::begin(){
    return JSONIterator( children );
}

jpp::JSONIterator jpp::JSON::end(){
    return JSONIterator( 0 );
}

jpp::JSON::Type jpp::JSON::getType(){
    return type;
}

void jpp::JSON::setNull(){
    for ( jpp::JSON *node( children ), *next( 0 ); node != 0; node = next ){
        next = node->next;
        delete node;
    }
    type = Type::Null;
    value = "null";
    children = 0;
}

bool jpp::JSON::remove(){
    if ( parent ){
        if ( !previous ){
            parent->children = next;
        }else{
            previous->next = next;
        }
        if ( next ){
            next->previous = previous;
        }
        delete this;
        return true;
    }
    return false;
}

void jpp::JSON::operator = ( const std::string &value ){
    if ( type != Type::String ){
        setNull();
        type = Type::String;
    }
    this->value = value;
}

void jpp::JSON::operator = ( const char *value ){
    if ( type != Type::String ){
        setNull();
        type = Type::String;
    }
    this->value = value;
}

void jpp::JSON::operator = ( const bool &value ){
    if ( type != Type::Boolean ){
        setNull();
        type = Type::Boolean;
    }
    this->value = value ? "true" : "false";
}

void jpp::JSON::operator = ( const int &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const unsigned int &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const long &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const unsigned long &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const long long &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const unsigned long long &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const float &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const double &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const long double &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const jpp::JSON &other ){
    setNull();
    type = other.type;
    value = other.value;
    if ( other.children ){
        for ( JSON *node( other.children ), *json( 0 ); node != 0; node = node->next ){
            if ( !children ){
                json = children = new JSON( *node );
            }else{
                json->next = new JSON( *node );
                json->next->previous = json;
                json = json->next;
            }
            json->key = node->key;
        }
    }
}

std::string jpp::JSON::getString(){
    return value;
}

bool jpp::JSON::getBoolean(){
    return value == "true";
}

int jpp::JSON::getInteger(){
    std::stringstream buffer( value );
    int number( 0 );
    buffer >> number;
    return number;
}

unsigned int jpp::JSON::getUInteger(){
    std::stringstream buffer( value );
    unsigned int number( 0 );
    buffer >> number;
    return number;
}

long jpp::JSON::getLong(){
    std::stringstream buffer( value );
    long number( 0 );
    buffer >> number;
    return number;
}

unsigned long jpp::JSON::getULong(){
    std::stringstream buffer( value );
    unsigned long number( 0 );
    buffer >> number;
    return number;
}

long long jpp::JSON::getLLong(){
    std::stringstream buffer( value );
    long long number( 0 );
    buffer >> number;
    return number;
}

unsigned long long jpp::JSON::getULLong(){
    std::stringstream buffer( value );
    unsigned long long number( 0 );
    buffer >> number;
    return number;
}

float jpp::JSON::getFloat(){
    std::stringstream buffer( value );
    float number( 0 );
    buffer >> number;
    return number;
}

double jpp::JSON::getDouble(){
    std::stringstream buffer( value );
    double number( 0 );
    buffer >> number;
    return number;
}

long double jpp::JSON::getLDouble(){
    std::stringstream buffer( value );
    long double number( 0 );
    buffer >> number;
    return number;
}

jpp::JSON &jpp::JSON::operator [] ( const std::string &key ){
    if ( type != Type::Object ){
        setNull();
        type = Type::Object;
    }
    JSON *node( children );
    int cmp( -1 );
    while ( node && ( cmp = strcmp( key.c_str(), node->key.c_str() ) ) > 0 && node->next ){
        node = node->next;
    }
    if ( cmp < 0 ){
        JSON *json( new JSON );
        if ( children == node ){
            children = json;
        }else{
            json->previous = node->previous;
            node->previous->next = json;
        }
        if ( node ){
            node->previous = json;
        }
        json->next = node;
        json->key = key;
        json->parent = this;
        return ( children == json ? *children : *json->previous->next );
    }else if ( cmp == 0 ){
        return ( children == node ? *children : *node->previous->next );
    }
    node->next = new JSON;
    node->next->key = key;
    node->next->parent = this;
    node->next->previous = node;
    return *node->next;
}

jpp::JSON &jpp::JSON::operator [] ( const size_t &index ){
    if ( type != Type::Array ){
        setNull();
        type = Type::Array;
    }
    JSON *node( children );
    size_t i( 0 );
    do{
        if ( !node || ( !node->next && i < index ) ){
            if ( !node ){
                children = ( node = new JSON );
            }else{
                node->next = new JSON;
                node->next->previous = node;
                node = node->next;
            }
            node->parent = this;
        }else if ( i != index ){
            node = node->next;
        }
    }while ( i++ != index );
    return ( children == node ? *children : *node->previous->next );
}

void jpp::JSON::parse( const std::string &json ){
    char *c( const_cast< char* >( &json[ 0 ] ) );
    readValue( c );
}

std::string jpp::JSON::toString(){
    std::string result;
    switch ( type ){
        case Type::String:
            {
                std::string buffer;
                for ( char c : value ){
                    if ( c == '"' ){
                        buffer += '\\';
                    }
                    buffer += c;
                }
                result += '"' + buffer + '"';
            }
        break;
        case Type::Object:
        case Type::Array:
            {
                bool &&isObject( type == Type::Object );
                result += isObject ? '{' : '[';
                for ( JSON *node( children ); node; node = node->next ){
                    if ( isObject ){
                        result += '"' + node->key + "\":";
                    }
                    result += node->toString();
                    if ( node->next ){
                        result += ',';
                    }
                }
                result += isObject ? '}' : ']';
            }
        break;
        default:
            result += value;
        break;
    }
    return result;
}

void jpp::JSON::readObject( char *&c ){
    while ( *c != 0 ){
        readAttribute( c );
        if ( *c != 0 ){
            if ( *c != ',' ){
                ++c;
                break;
            }
            ++c;
        }
    }
}

void jpp::JSON::readArray( char *&c ){
    size_t index( 0 );
    while ( *c != 0 ){
        this->operator [] ( index++ ).readValue( c );
        if ( *c != 0 ){
            if ( *c != ',' ){
                ++c;
                break;
            }
            ++c;
        }
    }
}

void jpp::JSON::readAttribute( char *&c ){
    while ( *c != 0 && *c != '"' ){
        ++c;
    }
    if ( *c != 0 ){
        std::string &&key( readString( ++c ) );
        if ( *c != 0 ){
            while ( *c != 0 && *c != ':' ){
                ++c;
            }
            if ( *c != 0 ){
                this->operator [ ] ( key ).readValue( ++c );
            }
        }
    }
}

void jpp::JSON::readValue( char *&c ){
    std::string s( "\t\n\v\f\r " );
    while ( *c != 0 && s.find_first_of( *c ) != std::string::npos ){
        ++c;
    }
    if ( *c != 0 ){
        switch ( *c ){
            case '"':
                this->operator = ( readString( ++c ) );
            break;
            case '{':
                readObject( ++c );
            break;
            case '[':
                readArray( ++c );
            break;
            default:
                std::string value;
                while ( *c != 0 && *c != ',' && *c != '}' && *c != ']' ){
                    if ( s.find_first_of( *c ) == std::string::npos ){
                        value += *c;
                    }
                    ++c;
                }
                if ( !value.empty() ){
                    if ( value == "true" ){
                        this->operator = ( true );
                    }else if ( value == "false" ){
                        this->operator = ( false );
                    }else if ( value != "null" ){
                        this->operator = ( value );
                        this->type = Type::Number;
                    }
                }
            break;
        }
    }
}

std::string jpp::JSON::readString( char *&c ){
    std::string result;
    while ( *c != 0 && *c != '"' ){
        result += *c != '\\' ? *c : readEscapeSequence( ++c );
        if ( *c != 0 ){
            ++c;
        }
    }
    if ( *c != 0 ){
        ++c;
    }
    return result;
}

char jpp::JSON::readEscapeSequence( char *&c ){
    char result( '?' );
    if ( *c != 0 ){
        switch ( *c ){
            case 'x':
            case 'u':
            case 'U':
                {
                    char code( 0 );
                    int i( 0 ), count( *c == 'x' ? 2 : ( *c == 'u' ? 4 : *c == 'U' ? 8 : 0 ) );
                    while ( i++ < count && *( ++c ) != 0 ){
                        code = code * 16 + ( *c >= '0' && *c <= '9' ? *c - '0' : ( *c >= 'a' && *c <= 'f' ? *c + 10 - 'a' : ( *c >= 'A' && *c <= 'F' ? *c + 10 - 'A' : 0 ) ) );
                    }
                    result = code;
                }
            break;
            default:
                result = *c;
            break;
        }
    }
    return result;
}

jpp::JSONIterator::JSONIterator( jpp::JSON *json ) :
    json( json ){
}

jpp::JSONIterator::JSONIterator( const jpp::JSONIterator &other ) :
    json( other.json ){
}

jpp::JSONIterator &jpp::JSONIterator::operator ++ (){
    json = json->next;
    return *this;
}

jpp::JSONIterator jpp::JSONIterator::operator ++ ( int ){
    return JSONIterator( json->next );
}

bool jpp::JSONIterator::operator == ( const jpp::JSONIterator &other ){
    return json == other.json;
}

bool jpp::JSONIterator::operator != ( const jpp::JSONIterator &other ){
    return json != other.json;
}

jpp::JSON &jpp::JSONIterator::operator * (){
    return *json;
}