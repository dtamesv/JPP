#include <JSON.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <codecvt>

int main( int c, char **args ){
    std::string path( args[ 0 ] );
    path.erase( path.find_last_of( '\\' ) );

    sf::Http http( "localhost", 80 );
    char option;

    std::wstring_convert< std::codecvt_utf8< wchar_t >, wchar_t > convert;

    while ( true ){
        std::cout << "( 1 ) : SAVE" << std::endl;
        std::cout << "( 2 ) : UPDATE" << std::endl;
        std::cout << "( 3 ) : DELETE" << std::endl;
        std::cout << "( 4 ) : DELETE ALL" << std::endl;
        std::cout << "( 5 ) : GET" << std::endl;
        std::cout << "( 6 ) : GET ALL" << std::endl;
        std::cout << "( 7 ) : CLOSE" << std::endl;

        std::cout << std::endl << "OPTION: ";
        option = getch();
        std::cout << option << std::endl << std::endl;

        switch ( option ){
            case '1':
                {
                    std::wstring nombre, apellido_1, apellido_2;
                    int dia( 0 ), mes( 0 ), anno( 0 );
                    std::cout << "Name: ";
                    std::wcin >> nombre;
                    std::cout << "Last name 1: ";
                    std::wcin >> apellido_1;
                    std::cout << "Last name 2: ";
                    std::wcin >> apellido_2;
                    std::cout << "Birthday:" << std::endl;
                    std::cout << "  Day: ";
                    std::cin >> dia;
                    std::cout << "  Month: ";
                    std::cin >> mes;
                    std::cout << "  Year: ";
                    std::cin >> anno;
                    std::cout << std::endl;

                    jpp::JSON query;
                    query[ "query" ][ "action" ] = "save";
                    query[ "query" ][ "data" ][ "nombre" ] = convert.to_bytes( nombre );
                    query[ "query" ][ "data" ][ "apellido_1" ] = convert.to_bytes( apellido_1 );
                    query[ "query" ][ "data" ][ "apellido_2" ] = convert.to_bytes( apellido_2 );
                    query[ "query" ][ "data" ][ "nac_dia" ] = dia;
                    query[ "query" ][ "data" ][ "nac_mes" ] = mes;
                    query[ "query" ][ "data" ][ "nac_anno" ] = anno;

                    sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );
                    jpp::JSON res( http.sendRequest( request ).getBody() );

                    if ( res[ "success" ].getBoolean() ){
                        std::cout << "SUCCESS: " << res[ "message" ].getString() << std::endl;
                        std::cout << "ID: " << res[ "data" ][ "id" ].getInteger() << std::endl;
                    }else{
                        std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                    }
                }
            break;
            case '2':
                {
                    int id( 0 );
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cout << std::endl;

                    jpp::JSON query;
                    query[ "query" ][ "action" ] = "get";
                    query[ "query" ][ "data" ][ "id" ] = id;

                    sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );

                    jpp::JSON res( http.sendRequest( request ).getBody() );

                    if ( res[ "success" ].getBoolean() ){
                        jpp::JSON row( res[ "data" ] );

                        do{
                            system( "cls" );
                            std::cout << "SUCEESS: " << res[ "message" ].getString() << std::endl;

                            std::cout << "( " << row[ "id" ].getInteger() << " ) " << row[ "nombre" ].getString() << " " << row[ "apellido_1" ].getString() << " " << row[ "apellido_2" ].getString() << " [ " << row[ "nac_dia" ].getInteger() << "/" << row[ "nac_mes" ].getInteger() << "/" << row[ "nac_anno" ].getInteger() << " ]" << std::endl << std::endl;

                            std::cout << "( 1 ) : NAME" << std::endl;
                            std::cout << "( 2 ) : LAST NAME 1" << std::endl;
                            std::cout << "( 3 ) : LAST NAME 2" << std::endl;
                            std::cout << "BIRTHDAY:" << std::endl;
                            std::cout << " ( 4 ) : DAY" << std::endl;
                            std::cout << " ( 5 ) : MONTH" << std::endl;
                            std::cout << " ( 6 ) : YEAR" << std::endl;
                            std::cout << "( 7 ) : SAVE" << std::endl;

                            option = getch();

                            switch ( option ){
                                case '1':
                                    {
                                        std::wstring nombre;
                                        std::cout << "Name: ";
                                        std::wcin >> nombre;
                                        row[ "nombre" ] = convert.to_bytes( nombre );
                                    }
                                break;
                                case '2':
                                    {
                                        std::wstring apellido_1;
                                        std::cout << "Lastname 1: ";
                                        std::wcin >> apellido_1;
                                        row[ "apellido_1" ] = convert.to_bytes( apellido_1 );
                                    }
                                break;
                                case '3':
                                    {
                                        std::wstring apellido_2;
                                        std::cout << "Lastname 2: ";
                                        std::wcin >> apellido_2;
                                        row[ "apellido_2" ] = convert.to_bytes( apellido_2 );
                                    }
                                break;
                                case '4':
                                    {
                                        int dia( 0 );
                                        std::cout << "Day: ";
                                        std::cin >> dia;
                                        row[ "nac_dia" ] = dia;
                                    }
                                break;
                                case '5':
                                    {
                                        int mes( 0 );
                                        std::cout << "Month: ";
                                        std::cin >> mes;
                                        row[ "nac_mes" ] = mes;
                                    }
                                break;
                                case '6':
                                    {
                                        int anno( 0 );
                                        std::cout << "Year: ";
                                        std::cin >> anno;
                                        row[ "nac_anno" ] = anno;
                                    }
                                break;
                            }
                        }while ( option != '7' );

                        query.setNull();
                        query[ "query" ][ "action" ] = "update";
                        query[ "query" ][ "data" ] = row;

                        sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );

                        res.setNull();
                        res.parse( http.sendRequest( request ).getBody() );

                        if ( res[ "success" ].getBoolean() ){
                            std::cout << "SUCCESS: " << res[ "message" ].getString() << std::endl;
                        }else{
                            std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                        }
                    }else{
                        std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                    }
                }
            break;
            case '3':
                {
                    int id( 0 );
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cout << std::endl;

                    jpp::JSON query;
                    query[ "query" ][ "action" ] = "delete";
                    query[ "query" ][ "data" ][ "id" ] = id;

                    sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );
                    jpp::JSON res( http.sendRequest( request ).getBody() );

                    if ( res[ "success" ].getBoolean() ){
                        std::cout << "SUCCESS: " << res[ "message" ].getString() << std::endl;
                    }else{
                        std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                    }
                }
            break;
            case '4':
                {
                    jpp::JSON query;
                    query[ "query" ][ "action" ] = "deleteAll";

                    sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );
                    jpp::JSON res( http.sendRequest( request ).getBody() );

                    if ( res[ "success" ].getBoolean() ){
                        std::cout << "SUCCESS: " << res[ "message" ].getString() << std::endl;
                    }else{
                        std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                    }
                }
            break;
            case '5':
                {
                    int id( 0 );
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cout << std::endl;

                    jpp::JSON query;
                    query[ "query" ][ "action" ] = "get";
                    query[ "query" ][ "data" ][ "id" ] = id;

                    sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );

                    jpp::JSON res( http.sendRequest( request ).getBody() );

                    if ( res[ "success" ].getBoolean() ){
                        std::cout << "SUCCESS: " << res[ "message" ].getString() << std::endl;
                        std::cout << "( " << res[ "data" ][ "id" ].getInteger() << " ) " << res[ "data" ][ "nombre" ].getString() << " " << res[ "data" ][ "apellido_1" ].getString() << " " << res[ "data" ][ "apellido_2" ].getString() << " [ " << res[ "data" ][ "nac_dia" ].getInteger() << "/" << res[ "data" ][ "nac_mes" ].getInteger() << "/" << res[ "data" ][ "nac_anno" ].getInteger() << " ]" << std::endl;
                    }else{
                        std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                    }
                }
            break;
            case '6':
                {
                    jpp::JSON query;
                    query[ "query" ][ "action" ] = "getAll";

                    sf::Http::Request request( "pruebaHTTP/index.php", sf::Http::Request::Post, query.toString() );

                    jpp::JSON res( http.sendRequest( request ).getBody() );

                    if ( res[ "success" ].getBoolean() ){
                        std::cout << "SUCCESS: " << res[ "message" ].getString() << std::endl;
                        for ( jpp::JSON &row : res[ "data" ] ){
                            std::cout << "( " << row[ "id" ].getInteger() << " ) " << row[ "nombre" ].getString() << " " << row[ "apellido_1" ].getString() << " " << row[ "apellido_2" ].getString() << " [ " << row[ "nac_dia" ].getInteger() << "/" << row[ "nac_mes" ].getInteger() << "/" << row[ "nac_anno" ].getInteger() << " ]" << std::endl;
                        }
                    }else{
                        std::cout << "ERROR: " << res[ "message" ].getString() << std::endl;
                    }
                }
            break;
            case '7':
                return 0;
            break;
        }
        std::cout << std::endl << "PRESS ANY KEY TO CONTINUE";
        getch();
        system( "cls" );
    }

    return 0;
}