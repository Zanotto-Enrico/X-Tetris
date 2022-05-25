#include "headers.h"

void Test()
{
    Partita* partita = malloc(sizeof(Partita));
    partita->Modalita = SINGLEPLAYER;
    int i,j;
    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++)
            partita->mappa[i][j] = 0;
    int continua = 1;
    while (continua)
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
            if(cmd[0] == 'k') 
                if(isPezzoPosizionabile(partita,pz))
                    confermaPezzo(partita,pz);
                else cmd[0] = ' ';
            if(cmd[0] == 'r') ruotaPezzo(partita, pz);
        }
        rimuoviLinee(partita);
        if(checkIfLost(partita)) continua = 0;
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

int isPezzoSpostabile(Partita* pa, Pezzo* pz)
{
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int result = 1;
    for ( i = 0; i < l*l; i++)
    {
        int x = pz->x + i%l;
        int y = pz->y + i/l;
        if(x > -1 && y > -1 && x < 10 && y < 15)
            result = !(*(pz->disposizione + i) && pa->mappa[y][x]) && result;
        else if(*(pz->disposizione + i) && result)
            result = !*(pz->disposizione + i);
    }
    return result;
}

int isPezzoPosizionabile(Partita* pa, Pezzo* pz)
{
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int result = 0;
    for ( i = 0; i < l*l; i++)
    {
        int x = pz->x + i%l;
        int y = pz->y + i/l;
        if(y == 14 && *(pz->disposizione + i)) result = 1;
        else if(x > -1 && y > -1 && x < 10 && y < 15)
            result = (*(pz->disposizione + i) && !*(pz->disposizione + l + i) 
                      && pa->mappa[y + 1][x]) || result;
    }
    return result;
}

int muoviPezzo(Partita* pa, Pezzo* pz, int dx, int dy)
{
    rimuoviPezzo(pa,pz);
    pz->x += dx; pz->y += dy;
    if(isPezzoSpostabile(pa,pz)) 
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
    if(isPezzoSpostabile(pa,pz))
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

int checkIfLost(Partita* pa)
{
    int i,j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 10; j++)
            if(pa->mappa[i][j]) return 1;
    return 0;
}