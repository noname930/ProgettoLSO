#ifndef funzioniDB_h
#define funzioniDB_h
#include <my_global.h>
#include <mysql.h> // gli header di MySQL C API

MYSQL *StartConnection();
void closeConnection(MYSQL *conn);
int verify_client_registration(MYSQL *conn,char *username, char* password);
int do_registration(MYSQL *conn,char *username, char* password);



#endif
