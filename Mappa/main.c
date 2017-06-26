#include <stdio.h>
#include <stdlib.h>
#include "mappa.c"
int main (int argc, char **argv)
{
  char **M=allocaMatrice();
  M=GeneraMappa();
  printf("\n\n");
  stampaMappa(M);
//  M=deallocaMappa(M);
  return 0;
}
