#include "headers.h"

void Test(Modalita mode)
{

    Partita* partita = malloc(sizeof(Partita));
    partita->Modalita = mode;
    partita->turno = P1;
    partita->stato = IN_CORSO;
    int i,j;
    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++)
            partita->mappa1[i][j] = 0;
            partita->mappa2[i][j] = 0;
    int continua = 1;
    while (partita->stato == IN_CORSO)
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
        char cmd = '0';
        while (cmd != '\n')
        {
            Update(partita);
            cmd = getchar();
            int fatta = 0;
            if(cmd == 'A') fatta = muoviPezzo(partita,pz,0,-1);
            if(cmd == 'D') fatta = muoviPezzo(partita,pz,-1,0);
            if(cmd == 'B') fatta = muoviPezzo(partita,pz,0,1);
            if(cmd == 'C') fatta = muoviPezzo(partita,pz,1,0);
            if(cmd == '\n') 
                if(isPezzoPosizionabile(partita,pz))
                    confermaPezzo(partita,pz);
                else cmd = ' ';
            if(cmd == 'r') ruotaPezzo(partita, pz);
        }
        rimuoviLinee(partita);
        if(checkIfLost(partita)) partita->stato = TERMINATA;
        distruggiPezzo(pz);
        if(partita->turno == P1) partita->turno = P2;
        else if(partita->turno == P2) partita->turno = P1;
        if(partita->Modalita == SINGLEPLAYER) partita->turno = P1;
    }
    
    Update(partita);

    free(partita);
}

void menuIniziale()
{
    Modalita mode = SINGLEPLAYER;
    char cmd = '0';
    while (cmd != '\n')
    {
        StampaMenuIniziale(mode);
        cmd = getchar();
        if(cmd == 'A' && mode == MULTIPLAYER)    mode = SINGLEPLAYER;
        else if(cmd == 'A' && mode == CPU)            mode = MULTIPLAYER;
        else if(cmd == 'B' && mode == SINGLEPLAYER)   mode = MULTIPLAYER;
        else if(cmd == 'B' && mode == MULTIPLAYER)    mode = CPU;
    }
    Test(mode);
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
        {
            if(pa->turno == P1) result = !(*(pz->disposizione + i) && pa->mappa1[y][x]) && result;
            else                result = !(*(pz->disposizione + i) && pa->mappa2[y][x]) && result;
        }
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
        {
            if(pa->turno == P1) result = (*(pz->disposizione + i) && pa->mappa1[y + 1][x] == 1) || result;
            else                result = (*(pz->disposizione + i) && pa->mappa2[y + 1][x] == 1) || result;
        }
            
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
        for(j = 0; j < 10; j++){
            if(pa->turno == P1) count = count + pa->mappa1[i][j];
            else                count = count + pa->mappa2[i][j];
        }
        if(count == 10)
        {
            for (j = 0; j < 10; j++){
                if(pa->turno == P1) pa->mappa1[i][j] = 0;
                else                pa->mappa2[i][j] = 0;
            }
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
            if(pa->turno == P1)
            {
                pa->mappa1[i+1][j] = pa->mappa1[i][j];
                pa->mappa1[i][j] = 0;
            }
            else 
            {
                pa->mappa2[i+1][j] = pa->mappa2[i][j];
                pa->mappa2[i][j] = 0;
            }
        }
    }
}

int checkIfLost(Partita* pa)
{
    int i,j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 10; j++)
            if(pa->mappa1[i][j] || pa->mappa2[i][j]) return 1;
    return 0;
}

void inserisciPezzo(Partita* pa, Pezzo* pz)
{
    ScriviSuMappa(pa,pz,2);
}

void rimuoviPezzo(Partita* pa, Pezzo* pz)
{
    ScriviSuMappa(pa,pz,0);
}

void confermaPezzo(Partita* pa, Pezzo* pz)
{
    ScriviSuMappa(pa,pz,1);
}

void ScriviSuMappa(Partita* pa, Pezzo* pz, int valore)
{
        int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int *parser = pz->disposizione;
    for (i = 0; i < l*l; i++)
        if(*(parser++) == 1)
        {
            if(pa->turno == P1) pa->mappa1[pz->y + i/l][pz->x + i%l] = valore;
            else                pa->mappa2[pz->y + i/l][pz->x + i%l] = valore;
        } 
    return;
}