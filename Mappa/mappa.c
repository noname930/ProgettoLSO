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
#define SIZE_COLONNE 75

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

        else if ((i==11 || i==12 || i==13 || i == 14 || i==15 || i == 16 || i == 17 || i == 18 || i == 19) && j == 10)
          M[i][j]='#';

        else if ((i==11 && j == 9) || (i == 19 && j == 9) || (i == 11 && j == 8 ) || (i == 19 && j == 8 ))
          M[i][j]='#';

        else if (((i==11 || i==12 || i==13 || i == 14 || i==15 || i == 16 || i == 17 || i == 18 || i == 19) && j == 65))
          M[i][j]='#';

        else if ((i==11 && j == 66) || (i == 19 && j == 66) || (i == 11 && j == 67 ) || (i == 19 && j == 67 ))
          M[i][j]='#';

        else
         M[i][j]=' ';

      }
    }

    generateObstacles(M);
    generateMines(M);
    generateWeapons(M);
    generateFlags(M);

    return M;
}

void generateWeapons(char **Mappa)
{

  int i,C_X,C_Y;
  int n_armi=rand() % 30;

  for(i=0;i<n_armi;i++)
  {
    C_X=(rand()%27)+1;
    C_Y=rand() % (64 - 11 + 1) + 11; // numeri compresi tra 11 e 64

    Mappa[C_X][C_Y]='A';      //Arma
  }

}

void generateObstacles(char **Mappa)
{
  srand(time(NULL));
  int i,C_X,C_Y;
  int n_ostacoli=rand() % 30 + 30;

  for(i=0;i<n_ostacoli;i++)
  {
    C_X=(rand()%27)+1;
    C_Y=rand() % (64 - 11 + 1) + 11; // numeri compresi tra 11 e 64


    Mappa[C_X][C_Y]='X';
  }

}

void generateMines(char **Mappa)
{

  int i,C_X,C_Y;
  int n_mine=rand() % 30;

  for(i=0;i<n_mine;i++)
  {
    C_X=(rand()%27)+1;
    C_Y=rand() % (64 - 11 + 1) + 11; // numeri compresi tra 11 e 64

    Mappa[C_X][C_Y]='*';      //Mina
  }

}


void generateFlags(char **Mappa)
{

  int i,C_X,C_Y;


    C_X=(rand()%27)+1; // da aggiustare
    C_Y=(rand()%35)+1; // da aggiustare

    Mappa[C_X][C_Y]='F';


    C_X=(rand()%27)+1; // idem
    C_Y=(rand()%35)+35; // idem

    Mappa[C_X][C_Y]='F';

}

void stampaMappa(char **M)
{
  int i,j;

  printf("\n");

    for(i=0;i<=SIZE_RIGHE;i++)
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

void inviaMappa(int socket, char **Mappa, int * status_client)
{
    int i,n=0;

      for(i=0;i<=SIZE_RIGHE;i++)
      {
        if((write(socket,Mappa[i],75)) == -1)
          *status_client=1;
      }

}

void removeClientFromMap(char **Mappa, int C_X , int C_Y)
{
    Mappa[C_X][C_Y]=' ';
}

void riceviMappa(int socket, char **Mappa)
{
    char c;
    int i,j;
    int nbytes;

      for(i=0;i<=SIZE_RIGHE;i++)
      {
         nbytes=read(socket,Mappa[i],75);
         Mappa[i][nbytes]='0';
      }


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
    *C_Y=30;
}


int movementCharacterClient(char **Mappa,char c, int *C_X, int *C_Y, int *punteggio)
{

  int fine=0;

//Qui dovremo fare P->punteggio--;
switch (c) {

 case 'a':
           if((*C_Y) - 1 != 0 && Mappa[*C_X][*C_Y -1 ] == ' ')
           {
             Mappa[*C_X][*C_Y]=' ';
             Mappa[*C_X][*C_Y - 1]='P';
             *C_Y=*C_Y-1;
             (*punteggio)--;
           }
           else if(Mappa[*C_X][*C_Y -1 ] == '*')
           {
               Mappa[*C_X][*C_Y]=' ';
               Mappa[*C_X][*C_Y - 1]='P';
               *C_Y=*C_Y-1;
               *punteggio=*punteggio - 600;
           }
           else if(Mappa[*C_X][*C_Y -1 ] == 'A')
           {
               Mappa[*C_X][*C_Y]=' ';
               Mappa[*C_X][*C_Y - 1]='P';
               *C_Y=*C_Y-1;
               *punteggio=*punteggio + 300;
           }
           else if(Mappa[*C_X][*C_Y -1 ] == 'F')
           {
               Mappa[*C_X][*C_Y]=' ';
               Mappa[*C_X][*C_Y - 1]='P';
               *C_Y=*C_Y-1;

             fine=1;
           }

           break;

 case 'w':
           if((*C_X) - 1 != 0 && Mappa[*C_X - 1][*C_Y]==' ')
           {
             Mappa[*C_X][*C_Y]=' ';
             Mappa[*C_X - 1 ][*C_Y]='P';
             *C_X=*C_X-1;
             (*punteggio)--;

           }
           else if(Mappa[*C_X - 1][*C_Y]=='*')
           {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X - 1 ][*C_Y]='P';
               *C_X=*C_X-1;
                *punteggio=*punteggio - 600;
           }
           else if(Mappa[*C_X - 1][*C_Y]=='A')
           {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X - 1 ][*C_Y]='P';
               *C_X=*C_X-1;
                *punteggio=*punteggio + 300;
           }
           else if(Mappa[*C_X - 1][*C_Y]=='F')
           {
               Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X - 1 ][*C_Y]='P';
               *C_X=*C_X-1;
                fine=1;
           }

           break;
 case 'd' :
           if(((*C_Y) + 1) != (SIZE_COLONNE - 1) && Mappa[*C_X][*C_Y + 1]==' ' )
           {
             Mappa[*C_X][*C_Y]=' ';
             Mappa[*C_X][*C_Y + 1]='P';
             *C_Y=*C_Y+1;
             (*punteggio)--;

           }
           else if(Mappa[*C_X][*C_Y + 1]=='*')
           {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X][*C_Y + 1]='P';
                *C_Y=*C_Y+1;
              *punteggio=*punteggio - 600;
           }
           else if(Mappa[*C_X][*C_Y + 1]=='A')
           {
                Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X][*C_Y + 1]='P';
                *C_Y=*C_Y+1;
                 *punteggio=*punteggio + 300;
           }
            else if(Mappa[*C_X][*C_Y + 1]=='F')
            {
                 Mappa[*C_X][*C_Y]=' ';
                Mappa[*C_X][*C_Y + 1]='P';
                *C_Y=*C_Y+1;
                 fine=1;
            }
           break;


 case 's' :
           if(((*C_X) + 1) != (SIZE_RIGHE-1) && Mappa[*C_X + 1][*C_Y] == ' ')
           {
             Mappa[*C_X][*C_Y]=' ';
             Mappa[*C_X + 1 ][*C_Y]='P';
             *C_X=*C_X+1;
             (*punteggio)--;

           }
           else if(Mappa[*C_X + 1][*C_Y] == '*')
           {
               Mappa[*C_X][*C_Y]=' ';
               Mappa[*C_X + 1 ][*C_Y]='P';
               *C_X=*C_X+1;
              *punteggio=*punteggio - 600;
           }
           else if(Mappa[*C_X + 1][*C_Y] == 'A')
           {
               Mappa[*C_X][*C_Y]=' ';
               Mappa[*C_X + 1 ][*C_Y]='P';
               *C_X=*C_X+1;
               *punteggio=*punteggio + 300;
           }
           else if(Mappa[*C_X + 1][*C_Y] == 'F')
           {
               Mappa[*C_X][*C_Y]=' ';
               Mappa[*C_X + 1 ][*C_Y]='P';
               *C_X=*C_X+1;
               fine=1;
           }

           break;
         }


         return fine;
}
