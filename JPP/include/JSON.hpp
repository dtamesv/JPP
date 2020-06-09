#ifndef JSON_HPP
#define JSON_HPP

#include <iterator>
#include <string>
#include <cstring>
#include <sstream>

namespace jpp{
    class JSONIterator;
    class JSON{
        friend JSONIterator;
        public:
            enum Type{
                Null = 0,
                String,
                Boolean,
                Number,
                Object,
                Array
            };

        private:
            Type type;
            std::string key;
            std::string value;
            JSON *children;
            JSON *parent;
            JSON *previous;
            JSON *next;

        public:
            JSON();
            JSON( const std::string &json );
            JSON( const JSON &other );
            ~JSON();

            JSONIterator begin();
            JSONIterator end();

            Type getType();

            void setNull();
            bool remove();

            void operator = ( const std::string &value );
            void operator = ( const char *value );
            void operator = ( const bool &value );
            void operator = ( const int &value );
            void operator = ( const unsigned int &value );
            void operator = ( const long &value );
            void operator = ( const unsigned long &value );
            void operator = ( const long long &value );
            void operator = ( const unsigned long long &value );
            void operator = ( const float &value );
            void operator = ( const double &value );
            void operator = ( const long double &value );
            void operator = ( const JSON &other );

            std::string getString();
            bool getBoolean();
            int getInteger();
            unsigned int getUInteger();
            long getLong();
            unsigned long getULong();
            long long getLLong();
            unsigned long long getULLong();
            float getFloat();
            double getDouble();
            long double getLDouble();

            JSON &operator [] ( const std::string &key );
            JSON &operator [] ( const size_t &index );

            void parse( const std::string &json );
            std::string toString();

        private:
            void readObject( char *&c );
            void readArray( char *&c );
            void readAttribute( char *&c );
            void readValue( char *&c );
            std::string readString( char *&c );
            char readEscapeSequence( char *&c );
    };

    class JSONIterator : public std::iterator< std::forward_iterator_tag, JSON, unsigned long, JSON*, JSON& >{
        friend JSON;
        private:
            JSON *json;
            JSONIterator( JSON *json );
        public:
            JSONIterator( const JSONIterator &other );
            JSONIterator &operator ++ ();
            JSONIterator operator ++ ( int );
            bool operator == ( const JSONIterator &other );
            bool operator != ( const JSONIterator &other );
            JSON &operator * ();
    };
}

#endif