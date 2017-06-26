#include "mappa.h"
#include <stdio.h>
#include <stdlib.h>
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
#include <netdb.h>
#include <time.h>
#define SIZE_RIGHE 30
#define SIZE_COLONNE 150

char** allocaMappa()
{
    int i;
    char **M=malloc(SIZE_RIGHE*sizeof(char *));

    for(i=0;i<SIZE_COLONNE;i++)
      M[i]=malloc(SIZE_COLONNE*sizeof(char));

    return M;
}


char** GeneraMappa()
{
    int i,j;

    char **M=allocaMappa();

    for(i=0;i<SIZE_RIGHE;i++)
    {
      for(j=0;j<SIZE_COLONNE;j++)
      {
        if(i == 0) //riga in alto
          M[i][j]='#';

        else if(j==0 && i!=0) // riga a sx
          M[i][j]='#';

        else if(j==(SIZE_COLONNE-1) && i != 0) // riga a dx
          M[i][j]='#';

        else if(i==(SIZE_RIGHE-1)) // riga sotto
         M[i][j]='#';

        else
         M[i][j]=' ';

      }
    }

    return M;
}


void stampaMappa(char **M)
{
  int i,j;

  printf("\n");

    for(i=0;i<SIZE_RIGHE;i++)
      printf("\n%s",M[i]);
}

char** deallocaMappa(char **M)
{
  int i;

  for(i=0;i<SIZE_RIGHE;i++)
        free(M[i]);

  free(M);

  return NULL;
}

void inviaMappa(int socket, char **Mappa)
{
    int i;

      for(i=0;i<=SIZE_RIGHE;i++)
        write(socket,Mappa[i],150);


}

void riceviMappa(int socket, char **Mappa)
{
    char c;
    int i,j;

      for(i=0;i<=SIZE_RIGHE;i++)
         read(socket,Mappa[i],150);




}

void spawnCharacterClient(char **Mappa, int* C_X , int* C_Y)
{
    int i,j,x,y;
    srand((unsigned int)time(NULL));


  /*  while(1)
    {
        x=rand()%(SIZE_RIGHE - 2) + 1;
        y=rand()%(SIZE_COLONNE - 2) + 1;

        if (Mappa[x][y]==' ')
        {
          Mappa[x][y]='P';
          break;
        }

    }*/

    Mappa[15][75]='P';
    *C_X=15;
    *C_Y=75;
}

void movementCharacterClient(char **Mappa,char c, int *C_X, int *C_Y)
{

  switch (c) {

    case 'a':
              if((*C_Y) - 1 != 0)
              {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X][*C_Y - 1]='P';
                *C_Y=*C_Y-1;
              }
              break;

    case 'w':
              if((*C_X) - 1 != 0)
              {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X - 1 ][*C_Y]='P';
                *C_X=*C_X-1;
              }
              break;
    case 'd' :
              if(((*C_Y) + 1) != (SIZE_COLONNE - 1))
              {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X][*C_Y + 1]='P';
                *C_Y=*C_Y+1;
              }
              break;


    case 's' :
              if(((*C_X) + 1) != (SIZE_RIGHE-1))
              {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X + 1 ][*C_Y]='P';
                *C_X=*C_X+1;
              }
              break;
            }
  }
