#include "headers.h"

void Test()
{
    Partita* partita = malloc(sizeof(Partita));
    partita->Modalita = SINGLEPLAYER;
    int i,j;
    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++)
            partita->mappa[i][j] = 0;

    while (1)
    {
        Pezzo *p = malloc(sizeof(Pezzo));
        p->x = 3; p->y = 0;
        p->disposizione = Z_PATTERN;
        p->tipo = Z;

        srand(time(NULL));
        int r = rand();

        switch (r%7)
        {
        case 0:
            p->disposizione = O_PATTERN;
            p->tipo = O;
            break;
        case 1:
            p->disposizione = L_PATTERN;
            p->tipo = L;
            break;
        case 2:
            p->disposizione = J_PATTERN;
            p->tipo = J;
            break;
        case 3:
            p->disposizione = I_PATTERN;
            p->tipo = I;
            break;
        case 4:
            p->disposizione = S_PATTERN;
            p->tipo = S;
            break;
        case 5:
            p->disposizione = Z_PATTERN;
            p->tipo = Z;
            break;
        case 6:
            p->disposizione = T_PATTERN;
            p->tipo = T;
            break;
        }
        muoviPezzo(partita,p,0,0);
        Update(partita);
        char cmd[2] = "";
        while (cmd[0] != 'k')
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
        free(p);
    }
    
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