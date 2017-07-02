
#include <stdio.h>
#include <string.h>
#include "funzioniDB.h"

char *server = "127.0.0.1";
char *user = "root";
char *password = "";
char *database = "alessio";


MYSQL *StartConnection()
{
  MYSQL *conn; //il puntatore alla connessione a mysql

  conn = mysql_init(NULL);

  if (!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
     fprintf(stderr, "%s\n", mysql_error(conn));
     exit(1);
  }

  return conn;

}

void closeConnection(MYSQL *conn)
{
   mysql_close(conn);
}

int verify_client_registration(MYSQL *conn,char *username, char* password)
{
  MYSQL_RES *res; //il puntatore alla risorsa mysql
  MYSQL_ROW row;  // il vettore in cui verranno messi i risultati delle query
  int trovato=0;
  char query[100];

  sprintf(query,"SELECT 'username' FROM utenti WHERE username='%s' AND password='%s'",username,password);

  printf("\nQuery=[%s]\n",query);

  if (mysql_query(conn,query)) {
     fprintf(stderr, "%s\n", mysql_error(conn));
     exit(1);
  }


  /*Prendo i risultati della query*/
  res = mysql_use_result(conn);

  /* Stampo a video i risultati della query */
  while ((row = mysql_fetch_row(res)) != NULL)
  {
    //nel vettore row ci sono i risultati adesso
    printf(" %s\n", row[0]);
    trovato++;
  }
  /* Distruggo il puntatore ai risultati della query
  e chiudo la connessione al database */
  mysql_free_result(res);

  return trovato;
}

int do_registration(MYSQL *conn,char *username, char* password)
{
  int trovato=0;
  char query[100];

  sprintf(query,"INSERT INTO `utenti`(`username`, `password`) VALUES ('%s','%s')",username,password);
  printf("\nQuery=[%s]\n\n",query);

  if (mysql_query(conn,query)) {
     //fprintf(stderr, "%s\n", mysql_error(conn));
     printf("\nUsername [%s] già in uso!\n", username);
     return 0;
  }
  else
  {
    printf("\nRegistrazione Username [%s] avvenuta con successo!", username);
    return 1;
  }
}


int verify_client_password(MYSQL *conn, char *password)
{
  MYSQL_RES *res; //il puntatore alla risorsa mysql
  MYSQL_ROW row;  // il vettore in cui verranno messi i risultati delle query
  int trovato=0;
  char query[100];

  sprintf(query,"SELECT 'password' FROM utenti WHERE password='%s'",password);

  printf("\nQuery=[%s]\n",query);

  if (mysql_query(conn,query)) {
     fprintf(stderr, "%s\n", mysql_error(conn));
     exit(1);
  }


  /*Prendo i risultati della query*/
  res = mysql_use_result(conn);

  /* Stampo a video i risultati della query */
  while ((row = mysql_fetch_row(res)) != NULL)
  {
    //nel vettore row ci sono i risultati adesso
    printf(" %s\n", row[0]);
    trovato++;
  }
  /* Distruggo il puntatore ai risultati della query
  e chiudo la connessione al database */
  mysql_free_result(res);

  return trovato;
}
