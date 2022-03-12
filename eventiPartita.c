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
        Pezzo *pz;
        srand(time(NULL));
        int r = rand();

        switch (r%7)
        {
            case 0: pz = generaPezzo(O); break;
            case 1: pz = generaPezzo(J); break;
            case 2: pz = generaPezzo(L); break;
            case 3: pz = generaPezzo(S); break;
            case 4: pz = generaPezzo(Z); break;
            case 5: pz = generaPezzo(I); break;
            case 6: pz = generaPezzo(T); break;
        }
        muoviPezzo(partita,pz,0,0);
        Update(partita);
        char cmd[2] = "";
        while (cmd[0] != 'k')
        {
            Update(partita);
            if(scanf("%1s", cmd) != 1) return;
            int fatta = 0;
            if(cmd[0] == 'A') fatta = muoviPezzo(partita,pz,0,-1);
            if(cmd[0] == 'D') fatta = muoviPezzo(partita,pz,-1,0);
            if(cmd[0] == 'B') fatta = muoviPezzo(partita,pz,0,1);
            if(cmd[0] == 'C') fatta = muoviPezzo(partita,pz,1,0);
            if(cmd[0] == 'k') confermaPezzo(partita,pz);
            if(cmd[0] == 'r') ruotaPezzo(partita, pz);
        }
        rimuoviLinee(partita);
        distruggiPezzo(pz);
    }
    
    Update(partita);

    free(partita);
}

Pezzo* generaPezzo(TipoPezzo tipo)
{
    Pezzo *pz = malloc(sizeof(Pezzo));
    pz->tipo = tipo;
    pz->x = 3; pz->y = 0;
    assegnaDisposizione(pz);
    return pz;
}

void distruggiPezzo(Pezzo* pz)
{
    free(pz->disposizione);
    free(pz);
}

void assegnaDisposizione(Pezzo* pz)
{
    int i, l = pz->tipo == O ? 4 : (pz->tipo == I ? 16 : 9);
    int *schema;
    switch (pz->tipo)
    {
        case O: schema = O_PATTERN; break;
        case I: schema = I_PATTERN; break;
        case Z: schema = Z_PATTERN; break;
        case S: schema = S_PATTERN; break;
        case L: schema = L_PATTERN; break;
        case T: schema = T_PATTERN; break;
        case J: schema = J_PATTERN; break;
    }
    pz->disposizione = malloc(sizeof(int)*l);
    for(i = 0; i < l; i++) pz->disposizione[i] = schema[i];
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

int ruotaPezzo(Partita* pa, Pezzo* pz)
{
    rimuoviPezzo(pa,pz);
    int i,j, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int *result = malloc(sizeof(int)*l*l);
    int n = 0;
    for(i = 0; i < l; i++)
    {
        for(j = l-1; j >= 0; j--)
        {
            result[n++] = pz->disposizione[j*l + i];
        }
    }
    int *old = pz->disposizione;
    pz->disposizione = result;
    if(isPezzoPosizionabile(pa,pz))
    {
        free(old);
        inserisciPezzo(pa,pz);
        return 1;
    }
    else
    {
        pz->disposizione = old;
        free(result);
        inserisciPezzo(pa,pz);
        return 0;
    }
}

int rimuoviLinee(Partita* pa)
{
    int i,j;
    for (i = 0; i < 15; i++)
    {
        int count = 0;
        for(j = 0; j < 10; j++)
            count = count + pa->mappa[i][j];
        if(count == 10)
        {
            for (j = 0; j < 10; j++)
                pa->mappa[i][j] = 0;
            abbassaBlocchi(pa,i);
        }   
    }
}

void abbassaBlocchi(Partita* pa, int linea)
{
    int i,j;
    for (i = linea-1; i > -1; i--)
    {
        for (j = 0; j < 10; j++)
        {
            pa->mappa[i+1][j] = pa->mappa[i][j];
            pa->mappa[i][j] = 0;
        }
    }
}