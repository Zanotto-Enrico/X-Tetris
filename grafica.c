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

void aggiornaCentroSchermo()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    CentroSchermo = w.ws_col/2;
}

void Stampa(char *s)
{
    printf("%*s", (int)CentroSchermo-(StrLenMB(s)/2), "");
    printf("%s",s);
}

void StampaL(char *s)
{
    Stampa(s);
    printf("\n");
}

void StampaLConColore(char *s, int NumColoriUsati)
{
    printf("%*s", (int)(CentroSchermo-((StrLenMB(s) - (NumColoriUsati/2 * 13))/2)), "");
    printf("%s",s);
    printf("\n");
}

void pulisciSchermo()
{
    gotoxy(0,0);
    char* vuoto = malloc(sizeof(char)*CentroSchermo*2+1);
    vuoto[CentroSchermo*2] = '\0';
    memset(vuoto,' ',CentroSchermo*2);
    int i;
    for (i = 0; i < 45; i++)
        Stampa(vuoto);
}

void StampaTitolo()
{
    StampaL(" __  __     ______   ______     ______   ______     __     ______    ");
    StampaL("/\\_\\_\\_\\   /\\__  _\\ /\\  ___\\   /\\__  _\\ /\\  == \\   /\\ \\   /\\  ___\\   ");
    StampaL("\\/_/\\_\\/_  \\/_/\\ \\/ \\ \\  __\\   \\/_/\\ \\/ \\ \\  __<   \\ \\ \\  \\ \\___  \\  ");
    StampaL("  /\\_\\/\\_\\    \\ \\_\\  \\ \\_____\\    \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\  \\/\\_____\\ ");
    StampaL("  \\/_/\\/_/     \\/_/   \\/_____/     \\/_/   \\/_/ /_/   \\/_/   \\/_____/ ");

    StampaL(" ");StampaL(" ");
 
}

void StampaMenuIniziale(Modalita mode)
{
    aggiornaCentroSchermo();
    pulisciSchermo();
    gotoxy(0,0);
    StampaTitolo();
    char Opt1[200] = "┃           ";
    char Opt2[200] = "┃           ";
    char Opt3[200] = "┃           ";
    char* S = "   SINGLE PLAYER              ┃";
    char* M = "    MULTYPLAYER               ┃";
    char* C = "        CPU                   ┃";
    char pointer[] = ">";
    char nopointer[] = " ";

    if(mode == SINGLEPLAYER) 
        strcat(Opt1,pointer);
    else 
        strcat(Opt1,nopointer);
    if(mode == MULTIPLAYER)
        strcat(Opt2,pointer);
    else 
        strcat(Opt2,nopointer);
    if(mode == CPU)
        strcat(Opt3,pointer);
    else 
        strcat(Opt3,nopointer);
    
    strcat(Opt1, S);
    strcat(Opt2, M);
    strcat(Opt3, C);

    StampaL("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    StampaL("┃                                          ┃");
    StampaL("┃      Seleziona la modalità di gioco      ┃");
    StampaL("┃                                          ┃");
    StampaL("┃                                          ┃");
    StampaL(Opt1);
    StampaL(Opt2);
    StampaL(Opt3);
    StampaL("┃                                          ┃");
    StampaL("┃                                          ┃");
    StampaL("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    StampaL("");
    StampaL(" Movimento: \'△ ▽\'        Ok: 'k' ");
}

void StampaModalita(Partita* partita)
{
    if(partita->Modalita == SINGLEPLAYER)
        StampaL("Modalita di gioco: SINGLE PLAYER");
    if(partita->Modalita == MULTIPLAYER)
        StampaL("Modalita di gioco: MULTYPLAYER");
        if(partita->Modalita == CPU)
    StampaL("Modalita di gioco: CPU vs UTENTE");
    StampaL(" ");StampaL(" ");
}

void Update(Partita *partita)
{
    aggiornaCentroSchermo();
    pulisciSchermo();
    gotoxy(0,0);
    StampaTitolo();
    StampaModalita(partita);
    StampaL("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    char riga[200] = "";
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
                if(i == 3) strcat(riga,"▁ ▁ ");
                else       strcat(riga,"    ");
                strcat(riga,ColoreDefault);
                coloriUsati += 2;
            }
            else if(i == 3) strcat(riga,"▁ ▁ ");
            else       strcat(riga,"    ");
        }
        strcat(riga," ┃");
        StampaLConColore(riga,coloriUsati);
        memset(riga, 0, sizeof(riga));
        coloriUsati = 0;
    }
    StampaL("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    StampaL("");
    StampaL(" Movimento: \'◁ ▷ △ ▽\'        Ok: 'k' ");
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