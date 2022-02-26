#include "headers.h"
#include "string.h"

int StrLenMB(char *s)
{
    int len = strlen(s);
    int count, i;
    count = 0;
    for(i = 0; i < len; i++)
        count += ((s[i] & 0xc0) != 0x80) ? 1 : 0;
    return count;
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", (y), (x));
}

void Stampa(char *s)
{
    printf("%*s", (int)CentroSchermo-(StrLenMB(s)/2), "");
    printf("%s",s);
    printf("\n");
}
void StampaConColore(char *s, int NumColoriUsati)
{
    printf("%*s", (int)(CentroSchermo-((StrLenMB(s) - (NumColoriUsati/2 * 13))/2)), "");
    printf("%s",s);
    printf("\n");
}

void StampaTitolo()
{
    int i;
    //for (i = 0; i < 50; i++)Stampa("");
    Stampa(" __  __     ______   ______     ______   ______     __     ______    ");
    Stampa("/\\_\\_\\_\\   /\\__  _\\ /\\  ___\\   /\\__  _\\ /\\  == \\   /\\ \\   /\\  ___\\   ");
    Stampa("\\/_/\\_\\/_  \\/_/\\ \\/ \\ \\  __\\   \\/_/\\ \\/ \\ \\  __<   \\ \\ \\  \\ \\___  \\  ");
    Stampa("  /\\_\\/\\_\\    \\ \\_\\  \\ \\_____\\    \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\  \\/\\_____\\ ");
    Stampa("  \\/_/\\/_/     \\/_/   \\/_____/     \\/_/   \\/_/ /_/   \\/_/   \\/_____/ ");

    Stampa("\n");
 
}

void StampaModalita(Partita* partita)
{
    if(partita->Modalita == SINGLEPLAYER)
        Stampa("Modalita di gioco: SINGLE PLAYER");
    if(partita->Modalita == MULTIPLAYER)
        Stampa("Modalita di gioco: MULTYPLAYER");
    Stampa("\n");
}

void Update(Partita *partita)
{
    gotoxy(0,0);
    StampaTitolo();
    StampaModalita(partita);
    Stampa("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    char riga[150] = "";
    int coloriUsati = 0;
    int i ,j;
    for (i = 0; i < 30; i++)
    {
        strcat(riga,"┃ ");
        for (j = 0; j < 10; j++)
        {
            if(partita->mappa[i/2][j] > 0)
            {
                if(partita->mappa[i/2][j] == 1)
                    strcat(riga,ColoreBianco);
                if(partita->mappa[i/2][j] == 2)
                    strcat(riga,ColoreBlu);
                strcat(riga,"    ");
                strcat(riga,ColoreDefault);
                coloriUsati += 2;
            }
            else strcat(riga,"    ");
        }
        strcat(riga," ┃");
        StampaConColore(riga,coloriUsati);
        memset(riga, 0, sizeof(riga));
        coloriUsati = 0;
    }
    Stampa("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    Stampa("");
    Stampa(" Movimento: \'◁ ▷ △ ▽\'        Ok: 'k' ");
    char*a = ""; 
    gotoxy(0,0);
    return;
}

void inserisciPezzo(Partita* pa, Pezzo* pz)
{
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int *parser = pz->disposizione;
    for (i = 0; i < l*l; i++)
        if(*(parser++) == 1) pa->mappa[pz->y + i/l][pz->x + i%l] = 2;
    return;
}

void rimuoviPezzo(Partita* pa, Pezzo* pz)
{
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int *parser = pz->disposizione;
    for (i = 0; i < l*l; i++)
        if(*(parser++) > 0) pa->mappa[pz->y + i/l][pz->x + i%l] = 0;
    return;
}

void confermaPezzo(Partita* pa, Pezzo* pz)
{
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    int *parser = pz->disposizione;
    for (i = 0; i < l*l; i++)
        if(*(parser++) == 1) pa->mappa[pz->y + i/l][pz->x + i%l] = 1;
    return;
}