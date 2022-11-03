#include "headers.h"
#include "string.h"

void gotoxy(int x, int y)
{
    /* utilizzo il carattere \033[<L>;<C>H per spostare il cursore */
    printf("\033[%d;%dH", (y), (x));
}

void aggiornaCentroSchermo()
{
    /*ottengo la larghezza del terminale chiamando la funzione */
    /* ioctl della libreria sys/ioctl.h e divido per 2 per il centroschermo */
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    CentroSchermo = w.ws_col/2;
}

int StrLenMB(char *s)
{
    int len = strlen(s);
    int count, i;
    count = 0;
    /* nel for conto solo i caratteri che fanno parte del range degli ASCII */
    /* evitando così di contare i codici del cambio colore */
    for(i = 0; i < len; i++)
        count += ((s[i] & 0xc0) != 0x80) ? 1 : 0;
    return count;
}

void Stampa(char *s)
{
    /* prima della stringa stampo una serie di spazi per allinearla al centro */
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
    /* prima della stringa stampo una serie di spazi per allinearla al centro */
    printf("%*s", (int)(CentroSchermo-((StrLenMB(s) - (NumColoriUsati/2 * 13))/2)), "");
    printf("%s",s);
    printf("\n");
}

void pulisciSchermo()
{
    /* costruisco una stringa di spazi lunga quanto il terminale */
    gotoxy(0,0);
    char* vuoto = malloc(sizeof(char)*CentroSchermo*2+1);
    vuoto[CentroSchermo*2] = '\0';
    memset(vuoto,' ',CentroSchermo*2);
    int i;
    /* stampo la stringa vuota più volte per pulire il terminale */
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

    /* la stringa da stampare varia in base alla scelta corrente dell'utente */
    char Opt1[200] = "┃           ";
    char Opt2[200] = "┃           ";
    char Opt3[200] = "┃           ";
    char* S = "   SINGLE PLAYER              ┃";
    char* M = "    MULTYPLAYER               ┃";
    char* C = "        CPU                   ┃";

    /* la posizione del puntatore dipende dalla scelta dell'utente */
    char pointer[] = ">";
    char nopointer[] = " ";


    if(mode == SINGLEPLAYER)     /* inserisco il puntatore alla prima opzione */
        strcat(Opt1,pointer);
    else 
        strcat(Opt1,nopointer);
    if(mode == MULTIPLAYER)      /* inserisco il puntatore alla seconda opzione */
        strcat(Opt2,pointer);
    else 
        strcat(Opt2,nopointer);
    if(mode == CPU)              /* inserisco il puntatore alla terza opzione */
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
    StampaL(Opt2);     /* stampo le 3 scelte */
    StampaL(Opt3);
    StampaL("┃                                          ┃");
    StampaL("┃                                          ┃");
    StampaL("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    StampaL("");
    StampaL(" Movimento: △ ▽        Ok: ⤶ ");
}

void StampaModalita(Partita* partita)
{
    /* stampo la modalità corrente al di sotto del titolo dell'applicazione */
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
    /* eseguo il refresh dell'intera schermata */
    aggiornaCentroSchermo();
    pulisciSchermo();
    gotoxy(0,0);
    StampaTitolo();
    StampaModalita(partita);
    int posizioneMenu;
    
    if(partita->Modalita == SINGLEPLAYER)
    {
        /* se la partita è singleplayer stampo una sola mappa*/
        stampaMappa(partita->mappa1, CentroSchermo-7, 11);
        /* in una partita singleplayer posiziono il menù a sx della mappa */
        posizioneMenu = CentroSchermo - 33;
    }
    else
    {
        /* se la partita è multiplayer stampo le due mappe */
        stampaMappa(partita->mappa1, CentroSchermo-59, 11);
        stampaMappa(partita->mappa2, CentroSchermo+15, 11);
        /* in una partita multiplayer posiziono il menù tra le due mappe */
        posizioneMenu = CentroSchermo - 11;
    }

    if(partita->stato == IN_CORSO)
    {
        /* se la partita è in corso stampo il menù di gioco */
        StampaTurno(&partita->turno, posizioneMenu);
        StampaMenuSceltaPezzo(partita->pezzoSelezionato,partita->pezziRimasti, posizioneMenu);
        StampaPunteggio(partita, posizioneMenu);
    }
    else
        /* se la partita è finita stampo il menù finale con il resoconto */
        stampaFine(partita, posizioneMenu);
}

void StampaMenuSceltaPezzo(TipoPezzo pz, int pezziRimasti[7], int x)
{
    /* prima stampo il riquadro che conterrà il pezzo */
    /* poi mi riposiziono al suo interno per stampare il pezzo */
    int y = 19;
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
    char letter = 'O';
    /* il pattern che devo stampare dipende dalla tipologia del pezzo attualmente */
    /* selezionato dall'utente. ottengo il pattern dalle variabili globali */
    switch (pz)
    {
        case O: pattern = O_PATTERN; letter = 'O'; break;
        case S: pattern = S_PATTERN; letter = 'S'; break;
        case T: pattern = T_PATTERN; letter = 'T'; break;
        case Z: pattern = Z_PATTERN; letter = 'Z'; break;
        case I: pattern = I_PATTERN; letter = 'I'; break;
        case J: pattern = J_PATTERN; letter = 'J'; break;
        case L: pattern = L_PATTERN; letter = 'L'; break;
    }
    gotoxy(x,21);
    printf("Tipo: %c    Rimasti: %d",letter, pezziRimasti[pz]);


    y = 24;                                         /* mi posiziono sempre ad altezza 24 */
    pz == O ? x+=7 : (pz == I ? x+=3 : (x+=5));     /* orizzontalmente mi posiziono in funzione del pezzo che devo stampare*/
    gotoxy(x,y);
    
    char stampa[400] = "";
    char* spazio = "    ";
    int i, l = pz == O ? 2 : (pz == I ? 4 : 3);
    for(i = 0; i < l*l && i < 8 ; i++)
    {
        if(*(pattern + i))  /*se il pattern in posizione i è uguale a 1 stampo con il colore*/
        {
            strcat(stampa,ColoreBianco); /* attivo il colore bianco */
            strcat(stampa,spazio);       /* concateno un blocco bianco alla stringa finale */
            strcat(stampa,ColoreDefault);/* attivo il colore default */
        }
        else                /*altrimenti stampo senza colore*/
        {
            strcat(stampa,spazio);      /* concateno un blocco vuoto alla stringa finale */
        }
        if((i+1)%l == 0)
        {
            /* stampo due volte perchè ogni "unità" che compone i blocchi è alta 2 caratteri */
            printf("%s",stampa);gotoxy(x,++y);
            printf("%s",stampa);gotoxy(x,++y);
            stampa[0] = '\0';
            
        }
    }
    printf("%s",stampa);
    gotoxy(0,0);
}


void StampaTurno(Giocatore* g, int x)
{
    int y = 12;     /* mi posiziono ad altezza 12 per stampare il turno */
    gotoxy(x,y);
    printf("     TURNO ATTUALE    ");gotoxy(x,++y);gotoxy(x,++y);

    if(*g == P1)    /* se è il turno del giocatore uno stampo giocatore 1 */
    {
        printf("    ◁ giocatore 1    ");gotoxy(x,++y);
    }
    if(*g == P2)    /* se è il turno del giocatore due stampo giocatore 2 */
    {
        printf("      giocatore 2 ▷  ");gotoxy(x,++y);
    }

    gotoxy(0,0);
}

void StampaPunteggio(Partita* pa, int x)
{
    int y = 35;     /* mi posiziono ad altezza 35 per stampare il punteggio */
    gotoxy(x,y);
    printf("      PUNTEGGIO     ");gotoxy(x,++y);gotoxy(x,++y);         /* ottengo il punteggio dei giocatori */
    printf("  Giocatore 1:  %d", pa->punteggio1);gotoxy(x,++y);         /*       dalla struct partita         */
    if(pa->Modalita == MULTIPLAYER) 
        printf("  Giocatore 2:  %d", pa->punteggio2);                   /* se sto giocando contro un altro    */
    if(pa->Modalita == CPU)                                             /* giocatore stampo giocatore 2       */
        printf("      CPU    :  %d", pa->punteggio2);                   /* altrimenti stampo CPU              */
    gotoxy(0,0);
}

void stampaMappa(int mappa[15][10], int x, int y)
{
    /* Questa funzione stampa la mappa fornitale costruendo ogni singola  */
    /* riga da stampare in base al contenuto della matrice mappa          */

    gotoxy(x,y);
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(x,++y);
    char riga[200] = "";                        /* questa è la riga che al termine di ogni ciclo rappresenterà la i-esima riga della mappa che stiamo stampando */
    int i ,j;
    int coloriUsati = 0;                
    for (i = 0; i < 30; i++)                    /* ogni riga della mappa è alta 2 caratteri quindi questo for viene eseguito 30 volte */
    {
        strcat(riga,"┃ ");                      /* inizio concatenando il bordo sinistro della mappa*/
                                                
        for (j = 0; j < 10; j++)                /*i "blocchi" contenuti in una riga della mappa sono 10, eseguo il ciclo per ognuno */
        { 
            if(mappa[i/2][j] > 0)               /* se la mappa in posizione [i/2][j] non è zero devo stampare un blocco colorato */
            {
                if(mappa[i/2][j] == 1)          /* se il valore è == 1 appartiene ad un blocco confermato */
                    strcat(riga,ColoreBianco);
                if(mappa[i/2][j] == 2)          /* se il valore è == 2 appartiene ad un blocco non ancora confermato */
                    strcat(riga,ColoreBlu);
                if(i == 3) strcat(riga,"▁ ▁ "); /* se staiamo stampando la 3a stringa aggiungo trateggio di limite superiore */
                else       strcat(riga,"    "); /* se è un qualunque altro blocco stampo solo uno spazio */

                strcat(riga,ColoreDefault);     /* in ogni caso resetto il colore */
                coloriUsati += 2;
            }
            else if(i == 3) strcat(riga,"▁ ▁ ");
            else       strcat(riga,"    ");     /* se invece il valore è zero aggiungo un blocco senza colorarlo */
        }
        strcat(riga," ┃");                      /* concludo la riga concatenando il bordo destro della mappa */

        printf("%s",riga);                      /*   stampo    */
        gotoxy(x,++y);                          /* cambio riga */
        memset(riga, 0, sizeof(riga));          /*  resestto   */
        coloriUsati = 0;
    }
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    gotoxy(x,y+2);                                                              /* stampo il bordo inferiore */
    printf(" Movimento: ◁ ▷ △ ▽   Ruota: r   Ok: ⤶ \n");                        /* e aggiungo una legenda    */
    gotoxy(0,0);
    return;
}

void stampaFine(Partita *pa, int x)
{
    /* stampo il resoconto finale in modo simile alla stampa degli altri menù */
    int i,y = 12;
    gotoxy(x,y);
    /*  cancello il menù di gioco stampato precendentemente */
    for (i = 0; i < 5; i++) 
    {
        printf("                      ");
        gotoxy(x,++y);
    }
    printf("  PARTITA TERMINATA!  ");gotoxy(x,++y);gotoxy(x,++y);
    printf("┏━━━━━━━━━━━━━━━━━━━━┓");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃  PUNTEGGIO FINALE  ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);
    printf("┃     Giocatore 1    ┃");gotoxy(x,++y);
    printf("┃         %02d         ┃",pa->punteggio1);gotoxy(x,++y);            /* ottengo il punteggio del giocatore 1 dalla struct */
    printf("┃                    ┃");gotoxy(x,++y);
    if(pa->Modalita != SINGLEPLAYER)                                            /* se non sono in modalità multiplayer non mostro i seguenti dati */
    {
        printf("┃     Giocatore 2    ┃");gotoxy(x,++y);
        printf("┃         %02d         ┃",pa->punteggio2);gotoxy(x,++y);        /* ottengo il punteggio del giocatore 2 dalla struct */
        printf("┃                    ┃");gotoxy(x,++y);
        printf("┃                    ┃");gotoxy(x,++y);
        printf("┃      VINCITORE     ┃");gotoxy(x,++y);
        if(pa->stato == VITTORIA_P1)
            printf("┃     Giocatore 1    ┃");                                   /* in base ha chi a vinto stampo giocatore 1 , 2 o CPU */
        if(pa->stato == VITTORIA_P2 && pa->Modalita == MULTIPLAYER)                                         
            printf("┃     Giocatore 2    ┃");
        if(pa->stato == VITTORIA_P2 && pa->Modalita == CPU)                                         
            printf("┃         CPU        ┃");
        gotoxy(x,++y);
        printf("┃                    ┃");gotoxy(x,++y);    
        printf("┃                    ┃");gotoxy(x,++y);   
    }
    printf("┃       Menu: ⤶      ┃");gotoxy(x,++y);
    printf("┃                    ┃");gotoxy(x,++y);    
    printf("┗━━━━━━━━━━━━━━━━━━━━┛");gotoxy(x,++y);


    /*  cancello il menù di gioco stampato precendentemente */
    for (i = 0; i < 5; i++)
    {
        printf("                      ");
        gotoxy(x,++y);
    }

    gotoxy(0,0);
}
