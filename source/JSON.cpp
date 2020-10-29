#include "JSON.hpp"

jpp::JSON::JSON() :
    type( Type::Null ),
    index( 0 ),
    value( "null" ),
    size( 0 ),
    height( 0 ),
    balanceFactor( 0 ),
    children( 0 ),
    parent( 0 ),
    root( 0 ),
    left( 0 ),
    right( 0 ){
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
    index = 0;
    parent = 0;
    root = 0;
    left = 0;
    right = 0;
}

jpp::JSONIterator jpp::JSON::begin(){
    JSON *from( 0 ), *json( children );
    while ( json && json->left ){
        from = json;
        json = json->left;
    }
    return JSONIterator( from, json );
}

jpp::JSONIterator jpp::JSON::end(){
    return JSONIterator( 0, 0 );
}

const jpp::JSON::Type &jpp::JSON::getType(){
    return type;
}

const std::string &jpp::JSON::getKey(){
    return key;
}

const size_t &jpp::JSON::getIndex(){
    return index;
}

const size_t &jpp::JSON::getSize(){
    return size;
}

void jpp::JSON::setNull(){
    type = Type::Null;
    value = "null";
    if ( children ){
        clear();
    }
}

void jpp::JSON::clear(){
    size = 0;
    height = 0;
    balanceFactor = 0;
    clear( children );
}

bool jpp::JSON::remove(){
    if ( parent ){
        JSON *node( left ? left : right );
        if ( node ){
            node->root = root;
        }
        if ( left && right ){
            JSON *aux( node );
            while ( aux->right ){
                aux = aux->right;
            }
            right->root = aux;
            aux->right = right;
        }
        if ( !root ){
            parent->children = node;
        }else if ( root->left == this ){
            root->left = node;
        }else{
            root->right = node;
        }
        balance( parent->children );
        --parent->size;
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

void jpp::JSON::operator = ( const short &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
}

void jpp::JSON::operator = ( const unsigned short &value ){
    if ( type != Type::Number ){
        setNull();
        type = Type::Number;
    }
    this->value = std::to_string( value );
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
    size = other.size;
    if ( other.children ){
        copy( children, other.children );
    }
}

const std::string &jpp::JSON::getString(){
    return value;
}

bool jpp::JSON::getBoolean(){
    return value == "true";
}

short jpp::JSON::getShort(){
    std::stringstream buffer( value );
    short number( 0 );
    buffer >> number;
    return number;
}

unsigned short jpp::JSON::getUShort(){
    std::stringstream buffer( value );
    unsigned short number( 0 );
    buffer >> number;
    return number;
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
    return *getChild( 0, children, key );
}

jpp::JSON &jpp::JSON::operator [] ( const size_t &index ){
    if ( type != Type::Array ){
        setNull();
        type = Type::Array;
    }
    return *getChild( 0, children, index );
}

bool jpp::JSON::isDefined( const std::string &key ){
    if ( type == Type::Object ){
        JSON *child( children );
        int cmp;
        while ( child && ( cmp = key.compare( child->key ) ) != 0 ){
            child = cmp < 0 ? child->left : child->right;
        }
        return child != 0;
    }
    return false;
}

bool jpp::JSON::isDefined( const size_t &index ){
    if ( type == Type::Array ){
        JSON *child( children );
        while ( child && index != child->index ){
            child = index < child->index ? child->left : child->right;
        }
        return child != 0;
    }
    return false;
}

void jpp::JSON::parse( const std::string &json ){
    setNull();
    char *c( const_cast< char* >( &json[ 0 ] ) );
    readValue( c );
}

std::string jpp::JSON::toString(){
    std::string result;
    switch ( type ){
        case Type::String:
            {
                result += '"';
                for ( char c : value ){
                    switch ( c ){
                        case '"':
                            result += "\\\"";
                        break;
                        case '\\':
                            result += "\\\\";
                        break;
                        case '\b':
                            result += "\\b";
                        break;
                        case '\f':
                            result += "\\f";
                        break;
                        case '\n':
                            result += "\\n";
                        break;
                        case '\r':
                            result += "\\r";
                        break;
                        case '\t':
                            result += "\\t";
                        break;
                        default:
                            result += c;
                        break;
                    }
                }
                result += '"';
            }
        break;
        case Type::Object:
        case Type::Array:
            {
                bool &&isObject( type == Type::Object );
                result += isObject ? '{' : '[';
                size_t index( 0 );
                bool first( true );
                for ( JSON &node : *this ){
                    if ( !first ){
                        result += ',';
                    }else{
                        first = false;
                    }
                    if ( isObject ){
                        result += '"' + node.key + "\":";
                    }else{
                        while ( index < node.index ){
                            result += "null,";
                            ++index;
                        }
                        ++index;
                    }
                    result += node.toString();
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

jpp::JSON *&jpp::JSON::getChild( jpp::JSON *root, jpp::JSON *&child, const std::string &key ){
    if ( !child ){
        child = new JSON;
        child->key = key;
        child->parent = this;
        if ( root ){
            child->root = root;
        }
        ++this->size;
        return child;
    }
    int cmp( key.compare( child->key ) );
    JSON *&json( cmp == 0 ? child : getChild( child, cmp < 0 ? child->left : child->right, key ) );
    json->recalculate();
    if ( json->balanceFactor < -1 ){
        if ( json->left->balanceFactor > 0 ){
            RR( json->left );
        }
        LL( json );
    }else if ( json->balanceFactor > 1 ){
        if ( json->right->balanceFactor < 0 ){
            LL( json->right );
        }
        RR( json );
    }
    return json;
}

jpp::JSON *&jpp::JSON::getChild( jpp::JSON *root, jpp::JSON *&child, const size_t &index ){
    if ( !child ){
        child = new JSON;
        child->index = index;
        child->parent = this;
        if ( root ){
            child->root = root;
        }
        ++this->size;
        return child;
    }
    JSON *&json( index == child->index ? child : getChild( child, index < child->index ? child->left : child->right, index ) );
    json->recalculate();
    if ( json->balanceFactor < -1 ){
        if ( json->left->balanceFactor > 0 ){
            RR( json->left );
        }
        LL( json );
    }else if ( json->balanceFactor > 1 ){
        if ( json->right->balanceFactor < 0 ){
            LL( json->right );
        }
        RR( json );
    }
    return json;
}

void jpp::JSON::balance( jpp::JSON *&root ){
    if ( root ){
        balance( root->left );
        balance( root->right );
        recalculate();
        if ( root->balanceFactor < -1 ){
            if ( root->left->balanceFactor > 0 ){
                RR( root->left );
            }
            LL( root );
        }else if ( root->balanceFactor > 1 ){
            if ( root->right->balanceFactor < 0 ){
                LL( root->right );
            }
            RR( root );
        }
    }
}

void jpp::JSON::recalculate(){
    long leftHeight( left ? left->height : -1 );
    long rightHeight( right ? right->height : -1 );
    height = std::max( leftHeight, rightHeight );
    balanceFactor = rightHeight - leftHeight;
}

void jpp::JSON::LL( jpp::JSON *&root ){
    JSON *aux( root->left->right );
    root->left->right = root;
    root->left->root = root->root;
    root->root = root->left;
    root = root->left;
    root->right->left = aux;
    root->right->recalculate();
    root->recalculate();
}

void jpp::JSON::RR( jpp::JSON *&root ){
    JSON *aux( root->right->left );
    root->right->left = root;
    root->right->root = root->root;
    root->root = root->right;
    root = root->right;
    root->left->right = aux;
    root->left->recalculate();
    root->recalculate();
}

void jpp::JSON::clear( jpp::JSON *&root ){
    if ( root ){
        clear( root->left );
        clear( root->right );
        root = 0;
        delete root;
    }
}

void jpp::JSON::copy( jpp::JSON *&root, jpp::JSON *other ){
    root = new JSON( *other );
    root->parent = this;
    root->key = other->key;
    root->index = other->index;
    root->height = other->height;
    root->balanceFactor = other->balanceFactor;
    if ( other->left ){
        copy( root->left, other->left );
        root->left->root = root;
    }
    if ( other->right ){
        copy( root->right, other->right );
        root->right->root = root;
    }
}

void jpp::JSON::readObject( char *&c ){
    std::string s( "\t\n\v\f\r " );
    while ( *c != 0 ){
        readAttribute( c );
        while ( *c != 0 && s.find_first_of( *c ) != std::string::npos ){
            ++c;
        }
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
    std::string s( "\t\n\v\f\r " );
    size_t index( 0 );
    while ( *c != 0 ){
        this->operator [] ( index++ ).readValue( c );
        while ( *c != 0 && s.find_first_of( *c ) != std::string::npos ){
            ++c;
        }
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
            case 'b':
                result = '\b';
            break;
            case 'f':
                result = '\f';
            break;
            case 'n':
                result = '\n';
            break;
            case 'r':
                result = '\r';
            break;
            case 't':
                result = '\t';
            break;
            default:
                result = *c;
            break;
        }
    }
    return result;
}

jpp::JSONIterator::JSONIterator( jpp::JSON *from, jpp::JSON *json ) :
    from( from ),
    json( json ){
}

jpp::JSONIterator::JSONIterator( const jpp::JSONIterator &other ) :
    from( other.from ),
    json( other.json ){
}

jpp::JSONIterator &jpp::JSONIterator::operator ++ (){
    if ( json->right && ( json->root == from || json->left == from ) ){
        from = json;
        json = json->right;
        while ( json->left ){
            this->from = json;
            json = json->left;
        }
    }else{
        do{
            from = json;
            json = json->root;
        }while ( json && json->right == from );
    }
    return *this;
}

jpp::JSONIterator jpp::JSONIterator::operator ++ ( int ){
    return ++JSONIterator( *this );
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