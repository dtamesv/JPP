# JPP: JSON Plus Plus Library
This is a small library aiming to be a very simple way to work with JSON objects in C++.

It ibrary allows the programmer to:
1. Parse JSON objects (from strings) and make them accessible as C++ JPP objects.
2. Build JSON objects (as strings) from C++ JPP objects.

`By Diego Tames Vargas.`

## Example or use:

### C++ to JSON

~~~ cpp
const jpp::JSON obj;

obj[ "name" ] = "Diego";
obj[ "lastname" ] = "Tames Vargas";
obj[ "job" ] = "Crazy Programmer!";

std::cout << "JSON: " << obj.toString() << std::endl;
~~~

Output:
~~~json
{"name":"Diego","lastname":"Tames Vargas","job":"Crazy Programmer!"}
~~~

### JSON to C++
~~~cpp
const jpp::JSON obj( "{\"name\":\"Diego\",\"lastname\":\"Tames Vargas\",\"job\":\"Crazy Programmer!\"}" );

std::cout << "Name: " << obj[ "name" ].getString() << std::endl;
std::cout << "Lastname: " << obj[ "lastname" ].getString() << std::endl;
std::cout << "Job: " << obj[ "job" ].getString() << std::endl;
~~~

Output:
~~~
Name: Diego
Lastname: Tames Vargas
Job: Crazy Programmer!
~~~
