#include "headers.h"

void IniziaPartita(Modalita mode)
{
    Partita* partita = inizializzaPartita(mode);
    while (partita->stato == IN_CORSO)
    {   
        richiediSceltaPezzo(partita);
        Pezzo *pz = generaPezzo(partita->pezzoSelezionato); 

        muoviPezzo(partita,pz,0,0);
        posizionaPezzo(partita,pz);

        distruggiPezzo(pz);
        
        int rimosse = rimuoviLinee(partita);
        if(partita->turno == P1) 
        {
            incrementaPunteggio(&partita->punteggio1, rimosse);
            if(rimosse >= 3 ) invertiRighe(partita, partita->mappa2, rimosse);
        }
        else
        {
            incrementaPunteggio(&partita->punteggio2, rimosse);
            if(rimosse >= 3 ) invertiRighe(partita, partita->mappa1, rimosse);
        }
        
        if(checkIfLost(partita)) partita->stato = TERMINATA;
        
        cambiaTurno(partita);
    }
    Update(partita);
    
    free(partita);
}

void posizionaPezzo(Partita* partita, Pezzo* pz)
{
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
}

void cambiaTurno(Partita* partita)
{
    if(partita->turno == P2 || partita->Modalita == SINGLEPLAYER) 
        partita->turno = P1;
    else
        partita->turno = P2;
}

void richiediSceltaPezzo(Partita* partita)
{
    partita->pezzoSelezionato = O;
    char cmd = '0';
    int scelto = 0;
    while (cmd != '\n' || partita->pezziRimasti[scelto] <= 0)
    {
        Update(partita);
        cmd = getchar();
        if(cmd == 'D') scelto = (scelto+1)%7;
        if(cmd == 'C') scelto = (scelto+6)%7;
        switch (scelto)
        {
            case 0: partita->pezzoSelezionato = O; break;
            case 1: partita->pezzoSelezionato = S; break;
            case 2: partita->pezzoSelezionato = T; break;
            case 3: partita->pezzoSelezionato = Z; break;
            case 4: partita->pezzoSelezionato = I; break;
            case 5: partita->pezzoSelezionato = J; break;
            case 6: partita->pezzoSelezionato = L; break;
        }
    }
    partita->pezziRimasti[scelto]--;
}

Partita *inizializzaPartita(Modalita mode)
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

    for (i = 0; i < 7; i++)
        partita->pezziRimasti[i] = 20;

    partita->punteggio1 = partita->punteggio2 = 0;
    partita->pezzoSelezionato = O;
    return partita;
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
    IniziaPartita(mode);
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
    int rimosse = 0;
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
            rimosse++;
            abbassaBlocchi(pa,i);
        }   
    }
    return rimosse;
}

void invertiRighe(Partita* pa, int mappa[15][10], int nrighe)
{
    int i = 0, j = 0;
    for (i = 14; i > 14-nrighe; i--)
        for (j = 0; j < 10; j++) 
            mappa[i][j] = (mappa[i][j]+1)%2;
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

void incrementaPunteggio(int* punteggio, int lineeRimosse)
{
    switch (lineeRimosse)
    {
        case 1:  *punteggio+=1;  break;
        case 2:  *punteggio+=3;  break;
        case 3:  *punteggio+=6;  break;
        case 4:  *punteggio+=12; break;
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