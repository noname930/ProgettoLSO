#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h> // gli header di MySQL C API
#include "funzioniDB.h"
#include <string.h>


int main (int argc, char **argv)
{
  MYSQL* conn=StartConnection();

  int val=verify_client_registration(conn,"bigb");

  printf("\nTrovato=%d username\n",val);

  printf("\nRegistro fausto - pippo93\n");
  do_registration(conn,"fausto","pippo89");
  do_registration(conn,"dio","pippo89");

  printf("\nFINE OPERAZIONI");


  return 0;
}
