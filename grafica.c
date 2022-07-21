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
    StampaL(" Movimento: △ ▽        Ok: ⤶ ");
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
    if(partita->Modalita == SINGLEPLAYER)
    {
        stampaMappa(partita->mappa1, CentroSchermo-22, 11);
    }
    else
    {
        stampaMappa(partita->mappa1, CentroSchermo-59, 11);
        stampaMappa(partita->mappa2, CentroSchermo+15, 11);
        StampaTurno(&partita->turno);
        StampaMenuSceltaPezzo(partita->pezzoSelezionato,partita->pezziRimasti);
        StampaPunteggio(partita);
    }
}

void StampaMenuSceltaPezzo(TipoPezzo pz, int pezziRimasti[7])
{
    int x = CentroSchermo-11, y = 19;
    gotoxy(x,y);
    printf("   Scegli un pezzo:   ");gotoxy(x,++y);gotoxy(x,++y);
    printf("Tipo: O    Rimasti: 0");gotoxy(x,++y);
    printf("┏━━━━━━━━━━━━━━━━━━━━┓");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┗━━━━━━━━━━━━━━━━━━━━┛");gotoxy(x,++y);
    printf("   Scegli:     ◁ ▷  ");gotoxy(x,++y);
    printf("   Conferma:     ⤶   ");gotoxy(x,++y);

    int* pattern;
    int numero = 0;
    switch (pz)
    {
        case O: pattern = O_PATTERN; numero = 0; break;
        case S: pattern = S_PATTERN; numero = 1; break;
        case T: pattern = T_PATTERN; numero = 2; break;
        case Z: pattern = Z_PATTERN; numero = 3; break;
        case I: pattern = I_PATTERN; numero = 4; break;
        case J: pattern = J_PATTERN; numero = 5; break;
        case L: pattern = L_PATTERN; numero = 6; break;
    }
    gotoxy(x,21);
    printf("Tipo: O    Rimasti: %d", pezziRimasti[numero]);


    x =CentroSchermo-11; y = 24;
    pz == O ? x+=7 : (pz == I ? x+=3 : (x+=5));
    gotoxy(x,y);
    
    char stampa[400] = "";
    char* spazio = "    ";
    int i, l = pz == O ? 2 : (pz == I ? 4 : 3);
    for(i = 0; i < l*l && i < 8 ; i++)
    {
        if(*(pattern + i))
        {
            strcat(stampa,ColoreBianco);
            strcat(stampa,spazio);
            strcat(stampa,ColoreDefault);
        }
        else
        {
            strcat(stampa,spazio);
        }
        if((i+1)%l == 0)
        {
            printf("%s",stampa);gotoxy(x,++y);
            printf("%s",stampa);gotoxy(x,++y);
            stampa[0] = '\0';
            
        }
    }
    printf("%s",stampa);
    


    gotoxy(0,0);
}


void StampaTurno(Giocatore* g)
{
    int x =CentroSchermo-11, y = 12;
    gotoxy(x,y);
    printf("     TURNO ATTUALE    ");gotoxy(x,++y);gotoxy(x,++y);
    if(*g == P1)
    {
        printf("    ◁ giocatore 1    ");gotoxy(x,++y);
    }
    if(*g == P2) 
    {
        printf("      giocatore 2 ▷  ");gotoxy(x,++y);
    }

    gotoxy(0,0);
}

void StampaPunteggio(Partita* pa)
{
        int x =CentroSchermo-11, y = 35;
    gotoxy(x,y);
    printf("      PUNTEGGIO     ");gotoxy(x,++y);gotoxy(x,++y);
    printf("  Giocatore 1:  %d", pa->punteggio1);gotoxy(x,++y);
    printf("  Giocatore 2:  %d", pa->punteggio2);gotoxy(x,++y);
    gotoxy(0,0);
}

void stampaMappa(int mappa[15][10], int x, int y)
{
    gotoxy(x,y);
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(x,++y);
    char riga[200] = "";
    int coloriUsati = 0;
    int i ,j;
    for (i = 0; i < 30; i++)
    {
        strcat(riga,"┃ ");
        for (j = 0; j < 10; j++)
        {
            if(mappa[i/2][j] > 0)
            {
                if(mappa[i/2][j] == 1)
                    strcat(riga,ColoreBianco);
                if(mappa[i/2][j] == 2)
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
        printf("%s",riga);
        gotoxy(x,++y);
        memset(riga, 0, sizeof(riga));
        coloriUsati = 0;
    }
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    gotoxy(x,y+2);
    printf(" Movimento: ◁ ▷ △ ▽   Ruota: r   Ok: ⤶ \n");
    gotoxy(0,0);
    return;
}

