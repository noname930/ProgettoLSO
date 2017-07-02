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
#include <signal.h>
#include "../Mappa/mappa.h"
#include "../Login/login.h"
#include "../mySQL/funzioniDB.h"
#include <my_global.h>
#include <mysql.h> // gli header di MySQL C API
#define BACKLOG 20

// comando per eseguire gcc serverTemp.c ../Mappa/mappa.c ../Login/login.c ../Input/inputReader.c -I/usr/include/mysql ../mySQL/funzioniDB.c -o server -lmysqlclient -lpthread && ./server
char **Mappa2;
int sd_global;


void *connection_handler(void *);
void     INThandler(int);

int main (int argc, char **argv)
{
  struct sockaddr_in serv_ind; // server indirizzo
  struct sockaddr_in client;  // client indirizzo
  socklen_t lung;
  in_port_t porta;
  int sd,val=1,accsd,i,j;
  char indcli[128],message[100];
  char str[INET6_ADDRSTRLEN];
  Mappa2=GeneraMappa();
  //stampaMappa(Mappa);
  signal(SIGINT, INThandler);


 //creazione del socket
  if((sd = socket(PF_INET,SOCK_STREAM,0)) < 0)
  {
    perror("Errore Creazione Socket");
    exit(-1);
  }

  sd_global=sd;

  // rende il socket riusabile
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));



  //========================CONFIG. INDIRIZZO IP=========================
  memset((void *)&serv_ind, 0, sizeof(serv_ind)); // pulisce ind
  serv_ind.sin_family = AF_INET; // ind di tipo INET
  serv_ind.sin_port = htons(8080); // scelgo porta non priv.
  serv_ind.sin_addr.s_addr = htonl(INADDR_ANY); // connessioni da ogni ip
//======================================================================
  puts("\n\t\tSTART SERVER!");
  sprintf(message, "Socket Server [%d]",sd);
  puts(message);

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
      inet_ntop(AF_INET, &client.sin_addr, indcli, sizeof(indcli));
      printf("Cliente %s connesso su Socket [%d]\n",indcli,accsd);

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
  int status_client=0;
//  char **Mappa=GeneraMappa();


  char *username,*password,message[50];
  int conta=0,reg=0,check_reg=0,check_pass=0;;
  MYSQL *conn=StartConnection();

//=====================LOGIN STATE==========================
    if(loginServer(sd,&username,&password))
    {
        if(verify_client_registration(conn,username,password))
        {
           printf("\nLogin Effettuato");
           write(sd,"Y",1);
           spawnCharacterClient(Mappa2,&Coordinata_X,&Coordinata_Y);
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
        spawnCharacterClient(Mappa2,&Coordinata_X,&Coordinata_Y);
      }

      else
      {
        printf("\nRegistrazione Fallita!\n");
        write(sd,"F",1);
      }
   }
//======================================================================

  int punteggio_client=5000;
  char punteggio_string[4];
  int fine=0;

  while(1)
  {
    inviaMappa(sd,Mappa2,&status_client);

      if(status_client)
      {
        sprintf(message,"Client_Socket[%d] Disconnect",sd);
        puts(message);
        removeClientFromMap(Mappa2,Coordinata_X,Coordinata_Y);
        break;
      }

    read(sd,&spostamento,1);

    if(spostamento != 'N' && (spostamento == 'a' || spostamento == 's' || spostamento == 'd' || spostamento == 'w'))
    {
     printf("\nRicevuto da Client_socket=%d spostamento a [%c]\n",sd,spostamento);
     fine=movementCharacterClient(Mappa2,spostamento,&Coordinata_X,&Coordinata_Y, &punteggio_client);
     if(fine==0)
     {
      sprintf(punteggio_string,"%d",punteggio_client);
      write(sd,punteggio_string,4);
     }
     else
      write(sd,"F",1);
    }

  sleep(1);

  }

  close(sd);
  pthread_exit(0);


}


void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
     printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
     {
          close(sd_global);
          exit(0);
     }
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}
