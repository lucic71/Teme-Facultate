# Tema3-Protocoale-de-Comuncatie
Implemented a web client that communicates with REST API. The server basically contains a library where the user
can store information about books. It supports operations like add_books, delete_books, etc.

It uses cookies to store a session and JWT tokens to restrict user access into the library.
The communication with the server is over the HTTP protocol, using GET POST and DELETE requests.
It uses cJSON library to work with JSON objects.
