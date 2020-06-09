// JSON Plus Plus - JPP
// Copyright (C) 2020 Diego Tames Vargas (thamesdiego@gmail.com)

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
            size_t index;
            std::string value;
            size_t size;
            long height;
            long balanceFactor;
            JSON *children;
            JSON *parent;
            JSON *root;
            JSON *left;
            JSON *right;

        public:
            JSON();
            JSON( const std::string &json );
            JSON( const JSON &other );
            ~JSON();

            JSONIterator begin();
            JSONIterator end();

            const Type &getType();
            const std::string &getKey();
            const size_t &getIndex();
            const size_t &getSize();

            void setNull();
            void clear();
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

            bool isDefined( const std::string &key );
            bool isDefined( const size_t &index );

            void parse( const std::string &json );
            std::string toString();

        private:
            JSON *&getChild( JSON *root, JSON *&child, const std::string &key );
            JSON *&getChild( JSON *root, JSON *&child, const size_t &index );
            void balance( JSON *&root );
            void recalculate();
            void LL( JSON *&root );
            void RR( JSON *&root );
            void clear( JSON *&root );
            void copy( JSON *&root, JSON *other );
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
            JSON *from;
            JSON *json;
            JSONIterator( JSON *from, JSON *json );
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