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

    Pezzo *p = malloc(sizeof(Pezzo));
    p->disposizione = Z_PATTERN;
    p->x = 3;
    p->y = 0;
    p->tipo = Z;
    muoviPezzo(partita,p,0,-1);

    char cmd[2] = "";
    while (1)
    {
        Update(partita);
        scanf("%1s", cmd);
        printf(cmd);
        int fatta = 0;
        if(cmd[0] == 'A') fatta = muoviPezzo(partita,p,0,-1);
        if(cmd[0] == 'D') fatta = muoviPezzo(partita,p,-1,0);
        if(cmd[0] == 'B') fatta = muoviPezzo(partita,p,0,1);
        if(cmd[0] == 'C') fatta = muoviPezzo(partita,p,1,0);
        if(cmd[0] == 'k') confermaPezzo(partita,p);
    }

    muoviPezzo(partita,p,0,1);
    

    Update(partita);


    free(partita);
}

int isPezzoPosizionabile(Partita* pa, Pezzo* pz)
{
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int posizionabile = 1;
    for ( i = 0; i < l*l; i++)
    {
        int x = pz->x + i%l;
        int y = pz->y + i/l;
        if(x > -1 && y > -1 && x < 10 && y < 15)
            posizionabile = !(*(pz->disposizione + i) && pa->mappa[pz->y + i/l][pz->x + i%l]) && posizionabile;
        else if(*(pz->disposizione + i) && posizionabile)
            posizionabile = !*(pz->disposizione + i);
    }
    return posizionabile;
}

int muoviPezzo(Partita* pa, Pezzo* pz, int dx, int dy)
{
    rimuoviPezzo(pa,pz);
    pz->x += dx; pz->y += dy;
    if(isPezzoPosizionabile(pa,pz)) 
    {
        inserisciPezzo(pa,pz);
        return 1;
    }
    else
    {
        pz->x -= dx; pz->y -= dy;
        inserisciPezzo(pa,pz);
        return 0;
    }
}