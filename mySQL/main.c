#include <my_global.h>
#include <mysql.h> // gli header di MySQL C API
#include <funzioniDB.h>
#include <stdio.h>
#include <string.h>

//comando per eseguire: gcc -I/usr/include/mysql main.c funzioniDB.c -o main -lmysqlclient

int main(void) {

   MYSQL *conn; //il puntatore alla connessione a mysql
   MYSQL_RES *res; //il puntatore alla risorsa mysql
   MYSQL_ROW row;  // il vettore in cui verranno messi i risultati delle query

  /* Dichiaro le variabili di connessione */
   char *server = "127.0.0.1";
   char *user = "root";
   char *password = "";
   char *database = "alessio";

   /*Inizializzo la connessione*/
   conn = mysql_init(NULL);

   /* Se possibile mi connetto al database altrimenti
   esco dal programma e scrivo un messaggio d'errore sullo stderr*/
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   /* Invio se possibile la query */
   if (mysql_query(conn, "SELECT * FROM utenti")) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   /*Prendo i risultati della query*/
   res = mysql_use_result(conn);

   /* Stampo a video i risultati della query */
   printf("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
   {
     //nel vettore row ci sono i risultati adesso
     printf("%s %s\n", row[0], row[1]);
   }

   /* Distruggo il puntatore ai risultati della query
   e chiudo la connessione al database */
   mysql_free_result(res);
   mysql_close(conn);

  return 0;
}
