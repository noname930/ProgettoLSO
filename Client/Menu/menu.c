#include <stdio.h>
#include <stdlib.h>
#include "../../Input/inputReader.c"



int Menu()
{
  int scelta;

  printf("\t\t\t\tBenvenuto su: BattleBash");
  printf("\nDigita una delle seguenti opzioni:");
  printf("\n\n1] Osserva la partita in Corso,\n
              2] Visualizza i Punteggi della partita in Corso,\n
              3] Seleziona la tua squadra di appartenenza e scendi in Campo.
              0] Exit\n==> ");
    do
    {
      scanf("%d",&scelta);

        if (scelta > 4)
          printf("\n[Opzione non valida]\n==> ");
    }while(scelta > 4)


}
