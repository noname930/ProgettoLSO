#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h> // Threads
#include <sys/socket.h> // Socket
#include <sys/un.h> // Connection
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "../Mappa/mappa.h"
#include "../Login/login.h"
#include "../mySQL/funzioniDB.h"
#include <my_global.h>
#include <mysql.h> // gli header di MySQL C API
#define BACKLOG 20




void *connection_handler(void *);

int main (int argc, char **argv)
{
  struct sockaddr_in serv_ind; // server indirizzo
  struct sockaddr_in client;  // client indirizzo
  socklen_t lung;
  in_port_t porta;
  int sd,val=1,accsd,i,j;
  char indcli[128];

  //stampaMappa(Mappa);


 //creazione del socket
  if((sd = socket(PF_INET,SOCK_STREAM,0)) < 0)
  {
    perror("Errore Creazione Socket");
    exit(-1);
  }

  // rende il socket riusabile
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));


  //========================CONFIG. INDIRIZZO IP=========================
  memset((void *)&serv_ind, 0, sizeof(serv_ind)); // pulisce ind
  serv_ind.sin_family = AF_INET; // ind di tipo INET
  serv_ind.sin_port = htons(8080); // scelgo porta non priv.
  serv_ind.sin_addr.s_addr = htonl(INADDR_ANY); // connessioni da ogni ip
//======================================================================


 //configurazione bind
  if (bind(sd, (struct sockaddr *)&serv_ind, sizeof(serv_ind)) < 0)
  {
    perror("Errore nella bind");
    exit(-1);
  }

//accetta fino a 20 connessioni, resta in ascolto sulla socket
  if (listen(sd, BACKLOG) < 0 )
  {
    perror("Errore nella listen");
    exit(-1);
  }

  char hello[50];
  pthread_t thread_id;
//  inet_ntop(AF_INET, &client.sin_addr, indcli, sizeof(indcli));
//  printf("Cliente %s connesso\n",indcli);

  while( (accsd = accept(sd, (struct sockaddr *)&client, &lung)) )
  {
      sprintf(hello,"Connection accepted from Client_socket: %d",accsd);
      puts(hello);

      if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &accsd) < 0)
       {
           perror("could not create thread");
           return 1;
       }

  }


  return 0;
}



/*================================THREAD====================================*/
void *connection_handler(void *socket_desc)
{
  int sd = *(int*)socket_desc;
  int Coordinata_X=0, Coordinata_Y=0;
  char spostamento;
  char **Mappa=GeneraMappa();
  spawnCharacterClient(Mappa,&Coordinata_X,&Coordinata_Y);


  char *username,*password;
  int conta=0,reg=0,check_reg=0,check_pass=0;;
  MYSQL *conn=StartConnection();

//=====================LOGIN STATE==========================
    if(loginServer(sd,&username,&password))
    {
        if(verify_client_registration(conn,username,password))
        {
          printf("\nLogin Effettuato");
           write(sd,"Y",1);
        }
        else
        {
         printf("\nLogin Failed: Utente o Password errati!");
          write(sd,"F",1);
        }

     }
     else
     {
      if(do_registration(conn,username,password))
      {
        printf("\nRegistrazione Effettuata!\n");
        write(sd,"Y",1);
      }

      else
      {
        printf("\nRegistrazione Fallita!\n");
        write(sd,"F",1);
      }
   }
//======================================================================


  while(1)
  {
    inviaMappa(sd,Mappa);
    read(sd,&spostamento,1);
    if(spostamento == 'a' || spostamento == 's' || spostamento == 'd' || spostamento == 'w')
    {
     printf("\nRicevuto da Client_socket=%d spostamento a [%c]\n",sd,spostamento);
     movementCharacterClient(Mappa,spostamento,&Coordinata_X,&Coordinata_Y);
    }
  }

}
