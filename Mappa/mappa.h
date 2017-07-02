#ifndef mappa_h
#define mappa_h

char** GeneraMappa();
char** allocaMappa();
void stampaMappa(char **);
void inviaMappa(int, char**, int *);
void riceviMappa(int, char **);
void spawnCharacterClient(char **, int*, int*);
void stampaMappa_Accessi(char **);
void removeClientFromMap(char **,int , int);
void generateObstacles(char**);
void generateFlags(char**);
void generateMines(char**);
void generateWeapons(char**);
int movementCharacterClient(char **,char , int *, int *, int *);
//char** deallocaMappa(char **M); da rivedere


#endif
