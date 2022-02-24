#include "headers.h"

void Test()
{
    Partita* partita = malloc(sizeof(Partita));
    partita->Modalita = SINGLEPLAYER;
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 10; j++)
            partita->mappa[i][j] = 0;
    
    partita->mappa[14][5] = 1;
    partita->mappa[14][6] = 1;
    partita->mappa[14][7] = 1;
    partita->mappa[13][6] = 1;

    partita->mappa[14][0] = 1;
    partita->mappa[14][1] = 1;
    partita->mappa[14][2] = 1;
    partita->mappa[13][2] = 1;

    partita->mappa[8][4] = 1;
    partita->mappa[7][4] = 1;
    partita->mappa[7][5] = 1;
    partita->mappa[6][5] = 1;
    

    Update(partita);
    free(partita);
}