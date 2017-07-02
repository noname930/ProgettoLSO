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

int temp_socket; // il socket del client è reso globale per poter effettuare letture e scritture nel thread movement_client
char **Mappa; // Mappa di gioco
char spostamento; // carattere che certifica quale spostamento ha fatto il player
int spostamento_effettuato=0; // verifica se il player ha digitato in input uno spostamento in game
int premuto_ctrl_c=0; // verifica se il client vuole uscire dalla partita
int scelta; // Scelta del Menu
int punteggio_client=5000;
int fine=0;


// comando per eseguire gcc clientTemp.c ../Mappa/mappa.c ../Login/login.c ../Input/inputReader.c -o client  -lpthread&& ./client
void  INThandler(int);
void *movement_client(void *socket);
void *update_map(void *socket);



int main (int argc, char **argv)
{
  int sd,sd2,i,j,nbyte; // socket id
  struct sockaddr_in serv_ind;
  struct hostent *ipserv;
  in_port_t porta;
  Mappa=allocaMappa();
  char *buff;
  pthread_t thread_id,thread_id2;
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



   //scelta=Menu();
/*
   if (scelta==1) // il player ha scelto di osservare la partita in corso
   {
     // lancio il thread che riceve la mappa
   }
   else if ( scelta == 2) // il player ha scelto di osservare i Punteggi
   {
     // lancio il menu dei punteggi
   }
   else if ( scelta == 0)
   {
     // un bel exit
   }
   else
   {
     // il player scende in battaglia ma prima seleziona la squadra
   }
*/

//thread che gestisce i movimenti del player in partita
//puts("Creato thread1");
if( pthread_create( &thread_id , NULL ,  movement_client , (void*) &sd) < 0)
 {
     perror("could not create thread");
     return 1;
 }

//thread che gestisce l'updating della mappa
//puts("Creato thread2");
 if( pthread_create( &thread_id2 , NULL ,  update_map , (void*) &sd) < 0)
  {
      perror("could not create thread");
      return 1;
  }

  void *status=0;

  pthread_join(thread_id,status);
  pthread_join(thread_id2,status);


close(sd);



  return 0;
}


void *movement_client(void *socket)
{
  int sd = *(int*)socket;
  int *status=0;
  char temp;

    while(premuto_ctrl_c == 0)
    {
      if(fine == 1 || punteggio_client <= 0)
        break;


        scanf(" %c", &temp);
        spostamento=temp;

        if(temp != 'y')
          printf("\nSpostamento fatto a [%c]", spostamento);

        spostamento_effettuato=1;

    }

    pthread_exit(status);
}


void  INThandler(int sig)
{
     char  c;
     premuto_ctrl_c=1;
     signal(sig, SIG_IGN);
     printf("\nExit!\n");
     exit(0);
}

void *update_map(void *socket)
{
  int sd = *(int*)socket;
  int *status;
  char buff_temp[4];
  //puts("sto in update");

  while(premuto_ctrl_c == 0)
  {
        if(fine == 1 || punteggio_client <= 0)
          break;

        system("clear");

        printf("[%d]\n",punteggio_client);

        riceviMappa(sd,Mappa);

        stampaMappa(Mappa);

        if(spostamento_effettuato)
        {
          write(sd,&spostamento,1);
          spostamento_effettuato=0;
          read(sd,buff_temp,4);

          if(strcmp(buff_temp,"F")==0)
          {
            fine=1;
          }
          else
            punteggio_client=atoi(buff_temp);
        }
        else
          write(sd,"N",1);

    sleep(1);
  }

  pthread_exit(status);
}
