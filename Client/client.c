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
#include "../Input/inputReader.h"
#define BACKLOG 20
#define RIGHE 30
#define COLONNE 150

// comando per eseguire gcc  server.c ../Mappa/mappa.c ../Login/login.c ../Input/inputReader.c -I/usr/include/mysql ../mySQL/funzioniDB.c -o server -lmysqlclient && ./server



int main (int argc, char **argv)
{
  int sd,i,j,nbyte; // socket id
  struct sockaddr_in serv_ind;
  struct hostent *ipserv;
  in_port_t porta;
  char **Mappa=allocaMappa();
  char *buff;




//==================CONF. INDIZZO IP===========
// indirizzo IP
memset((void *)&serv_ind, 0, sizeof(serv_ind)); // pulizia ind
serv_ind.sin_family = AF_INET; // ind di tipo INET
porta = 8080;
serv_ind.sin_port = htons(porta); // porta a cui collegarsi
inet_aton("95.247.151.91",&serv_ind.sin_addr);

if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
   perror("Errore in creazione socket");
   exit(-1);
}


// stabilisce la connessione
if (connect(sd, (struct sockaddr *) &serv_ind, sizeof(serv_ind)) < 0) {
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
  clearBuffer();
  scanf(" %c", &movimento);
  write(sd,&movimento,1);
}


close(sd);



  return 0;
}
