POPESCU LUCIAN IOAN 321CD
-------------------------

    TEMA 3 PROTOCOALE DE COMUNICATIE
    --------------------------------


I. File structure
-----------------

include/
authentication.h Contains a auth_info_t object
book.h           Contains a book_info_t object and a book_id_t object
buffer.h         Functions used in connection.c
cJSON.h          JSON parsing library
commands.h       Prototypes of the implemented commands
connection.h     Functions that deal with connection with the server
dns.h            Functions that send DNS requests
error.h          Macros used to print and deal with errors
http.h           Functions that extract info from HTTP headers, etc
memory.h         Macros used for safe memory management(CALLOC, MALLOC, FREE)
prompt.h         Extract data from prompts (for example: username, password)
requests.h       Functions that build GET, POST and DELETE requests
server.h         Information about the server (URLS, NAME, etc)
./
authentication.c Implements specific functionality
book.c           Implements specific functionality 
buffer.c         Implements specific functionality 
cJSON.c          JSON library implementation
client.c         Main driver for client
commands.c       Implements specific functionality 
connection.c     Implements specific functionality 
dns.c            Implements specific functionality 
http.c           Implements specific functionality 
prompt.c         Implements specific functionality 
requests.c       Implements specific functionality 
Makefile         Makefile to build the client
README           This file

II. Design
----------

A. Client and operations(commands)
----------------------------------

The implementation of the client is pretty straight forward. The client reads
input from stdin and using a huge block of if..else statements, it decides which
operation to make.

Each operation has three return status codes: OPERATION_SUCCESSFUL,
OPERATION_CONNECTION_CLOSED and OPERATION_FAILED. The client checks after each
operation the return code to make sure that the connection with the server is
still ok, if not it retries to re-establish it. It also checks that the
operations were successful in order to manage the correct creation and deletion
of cookies and JWT tokens.

The operations are implemented using the same rules:
    - fetch data from user if needed
    - build the HTTP request
    - send the request and receive the response
    - work with the response if needed

Moreover each operation takes care of the errors which may occur during the
above mentioned steps. For this each variable is declared at the beginning
of each opearion and if an error occurs the memory will be free'd for this
variables using a goto statement and a label at the end of the current
operation.

B. JSON
-------

For parsing, converting and working with JSON objects I used the cJSON library.
It is a simple library containing only a header file and a source file, that
can easily be inserted in the project.

I chose this library because it has a basic interface containing functions and
objects that require a small amount of time to understand, as the authors say:
"cJSON aims to be the dumbest possible parser that you can get your job done 
with".

More info and tutorials can be found on their github page:
"https://github.com/DaveGamble/cJSON".

C. Reading data from user
-------------------------

prompt.c is the file where this functionality is implemented. It pops up a
prompt for the required data and using a basic read function like fgets it
gets the job done. I also added password safety by turning off the echoing
when the password is inserted.

D. Other objects and functionality
----------------------------------

For keeping all as simple as possible, I encapsulated the information coming
from the user (username, password, book info) in objects that have their own
metods for parsing to JSON or deletion. Their location can be found in
I. File structure.

The functionality for connecting with the server and creating requests is
mainly borrowed from lab10 HTTP with some minor changes.

