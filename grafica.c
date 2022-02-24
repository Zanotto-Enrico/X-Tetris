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
    for (i = 0; i < 50; i++)Stampa("");
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
            if(partita->mappa[i/2][j])
            {
                strcat(riga,ColoreRosso);
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
    char*a = "◁ ▷ △ ▽"; 
    return;
}