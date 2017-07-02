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
#include "../Input/inputReader.h"
#define BACKLOG 20
#define RIGHE 30
#define COLONNE 150

int temp_socket;

// comando per eseguire gcc server.c ../Mappa/mappa.c ../Login/login.c ../Input/inputReader.c -I/usr/include/mysql ../mySQL/funzioniDB.c -o server -lmysqlclient && ./server
// comando per eseguire gcc client.c ../Mappa/mappa.c ../Login/login.c ../Input/inputReader.c -I/usr/include/mysql ../mySQL/funzioniDB.c -o client -lpthread  -lmysqlclient && ./client
void *update_map(void *socket);
void     INThandler(int);

int main (int argc, char **argv)
{
  int sd,sd2,i,j,nbyte; // socket id
  struct sockaddr_in serv_ind;
  struct hostent *ipserv;
  in_port_t porta;
  char **Mappa=allocaMappa();
  char *buff;
  pthread_t thread_id;
  signal(SIGINT, INThandler);



//==================CONF. INDIZZO IP===========
// indirizzo IP
memset((void *)&serv_ind, 0, sizeof(serv_ind)); // pulizia ind
serv_ind.sin_family = AF_INET; // ind di tipo INET
porta = 8080;
serv_ind.sin_port = htons(porta); // porta a cui collegarsi
inet_aton("87.1.143.203",&serv_ind.sin_addr);

if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
   perror("Errore in creazione socket");
   exit(-1);
}



temp_socket=sd;

// stabilisce la connessione
if (connect(sd, (struct sockaddr *) &serv_ind, sizeof(serv_ind)) == -1) {
perror("Errore nella connessione");
exit(-1);
}

//printf("Connessione stabilita con %s\n",inet_ntoa(serv_ind.sin_addr));
char movimento,scelta;
char *username,risposta_Server,tmp;
int conta=0;

  loginClient(sd,username);
  read(sd,&risposta_Server,1);

  if(risposta_Server == 'F')
  {
    printf("\nLogin Errato");
   exit(1);  // per il momento...
  }
  else
   printf("\nLogin EFFETTUATO!");


printf("\nPremi un tasto per continuare...\n");
clearBuffer();
tmp=getchar();


while(1)
{
  system("clear");
  riceviMappa(sd,Mappa);
  stampaMappa(Mappa);
  printf("\nChe movimento vuoi fare?[a-s-w-d]\n");
  //clearBuffer();
  scanf(" %c", &movimento);
  write(sd,&movimento,1);
}


close(sd);



  return 0;
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
          write(temp_socket,"@",1);
          exit(0);
     }
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}
