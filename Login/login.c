#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h> // Threads
#include <sys/socket.h> // Socket
#include <sys/un.h> // Connection
#include "../Input/inputReader.h"
#include "login.h"

#define BUFFER 100


void loginClient(int sd,char *username)
{
      char *password=malloc(10*sizeof(char));
            username=malloc(10*sizeof(char));
      char tmpchar[2],go[20];

      printf("\nIl tuo account è gia registrato? (y/n): ");
      scanf("%s",tmpchar);
      //*c=tmpchar;

      if( (write(sd,tmpchar,2)) < 0 )
      {
        perror("Error Write Require Registration on DB");
        exit(-1);
      }

      printf("\n[USERNAME]: ");
      scanf("%s",username);

      if( (write(sd,username,strlen(username))) < 0 )
      {
        perror("Error Write username to Server");
        exit(-1);
      }

      printf("\n[PASSWORD]: ");
      scanf("%s",password);

      if( (write(sd,password, strlen(password))) < 0 )
      {
        perror("Error Write Password to Server");
        exit(-1);
      }

       //   read(sd,go,20);
      // printf("\nMessaggio dal server: %s",go);


}



int loginServer(int accsd,char** username, char** password)
{
  char tmpchar[2];
    *username=malloc(10*sizeof(char));
    *password=malloc(10*sizeof(char));

    if( (read(accsd,tmpchar,2)) < 0 )
    {
      perror("Error reading registration on DB");
      exit(-1);
    }
      printf("\nLetto %s", tmpchar);

    if( (read(accsd,*username,10)) < 0 )
    {
      perror("Error Reading from Client:username");
      exit(-1);
    }
    printf("\nLetto %s", *username);


    if( (read(accsd,*password,10)) < 0 )
    {
      perror("Error reading from Client:password");
      exit(-1);
    }
    printf("\nLetto %s", *password);


    printf("\nRichiesta di login da [%s] [%s] [%c]",*username,*password,tmpchar[0]);

    if(strcmp(tmpchar,"y") == 0)
    {
    //  printf("\nUtente Registrato!");
      return 1;
    }
    else
    {
    //  printf("\nUtente non registrato!");
      return 0;
    }

  //  write(accsd, "<Server>: Login Effettuato",strlen("<Server>: Login Effettuato"));
    //sleep(5);
}
