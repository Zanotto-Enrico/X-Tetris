#include "headers.h"

void IniziaPartita(Modalita mode)
{
    Partita* partita = inizializzaPartita(mode);                       /* inizializzo la struct partita                     */
    while (partita->stato == IN_CORSO)                                 /* ciclo fino alla fine della partita                */
    {   
        Pezzo *pz = NULL;
        if(partita->Modalita == CPU && partita->turno == P2)           /* se è il turno del computer                        */
            eseguiMossaCPU(partita);                                   /* chiedo alla macchina di fare una mossa            */
        else
        {                                                              /* altrimenti, se è il turno di un giocatore         */
            richiediSceltaPezzo(partita);                              /* richiedo al giocatore di scegliere un pezzo       */
            pz = generaPezzo(partita->pezzoSelezionato);               /* genero un pezzo del tipo selezionato dall'utente  */
            muoviPezzo(partita,pz,0,0);                                /* posiziono il pezzo appena creato nella mappa      */
            posizionaPezzo(partita,pz);                                /* chiedo al giocatore di posizionarlo e confermarlo */
            distruggiPezzo(pz);                                        /* distruggo il pezzo che non serve più              */
        }
        
        int rimosse = rimuoviLinee(partita);                              /* verifico se ci sono righe da eliminare e salvo la quantità    */
        if(partita->turno == P1) 
        {
            incrementaPunteggio(&partita->punteggio1, rimosse);                       /* se il turno è del giocatore 1 aumento il suo punteggio in     */
            if(rimosse >= 3 ) invertiRighe(partita->mappa2, rimosse);                 /* base alle righe rimosse e eventualmente se il numero è        */
        }                                                                             /* maggiore di 3 inverto le medesime righe nel campo avversario  */
        else
        {
            incrementaPunteggio(&partita->punteggio2, rimosse);                       /* se il turno è del giocatore 2 aumento il suo punteggio in     */
            if(rimosse >= 3 ) invertiRighe(partita->mappa1, rimosse);                 /* base alle righe rimosse e eventualmente se il numero è        */
        }                                                                             /* maggiore di 3 inverto le medesime righe nel campo avversario  */
        
        cambiaTurno(partita);                                                         /* terminata la mossa cambio il turno                            */
        rimuoviLinee(partita);                                                        /* verifico se dopo l'inversione ci sono righe da rimuovere      */

        int end = checkGameEnd(partita);                                              /* verifico se la partita è giunta al termine                    */
        if(end == 1) partita->stato = VITTORIA_P2;                                    /* se la funzione ritorna 1 ha vinto il giocatore 2              */
        if(end == 2) partita->stato = VITTORIA_P1;                                    /* se ritnorna 2 ha vinto il giocatore 1                         */
    }
    Update(partita);                                                                  /* terminata la partita aggiorno lo schermo per il menù finale   */
    richiediSceltaPezzo(partita);                                                     /* utilizzo questa funzione per attendere un "invio" dall'utente */
    free(partita);                                                                    /* la partita è terminata quindi libero la memoria               */
}

void menuIniziale()
{
    Modalita mode = SINGLEPLAYER;
    char cmd = '0';     /* variabile per memorizzare l'input */
   
    while (cmd != '\n')             /* fino a che l'utente non preme invio attendo l'input */
    {
        StampaMenuIniziale(mode);   /* stampo la grafica del menù */
        cmd = getchar();            /*ottengo l'input utente*/
        if(cmd == 'A' && mode == MULTIPLAYER)    mode = SINGLEPLAYER;           /* se l'utente preme la freccia verso l'alto   */
        else if(cmd == 'A' && mode == CPU)            mode = MULTIPLAYER;       /* ciclo verso l'alto l'opzione selezionata    */

        else if(cmd == 'B' && mode == SINGLEPLAYER)   mode = MULTIPLAYER;       /* se l'utente preme la freccia verso il basso */
        else if(cmd == 'B' && mode == MULTIPLAYER)    mode = CPU;               /* ciclo verso il basso l'opzione selezionata  */
    }
    IniziaPartita(mode);
}

Partita *inizializzaPartita(Modalita mode)
{
    /* alloco memoria per la struct partita e setto le variabili principali */
    Partita* partita = malloc(sizeof(Partita));
    partita->Modalita = mode;
    partita->turno = P1;
    partita->stato = IN_CORSO;
    /* riempo entrambi i campi di gioco con zeri */
    int i,j;
    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++)
            partita->mappa1[i][j] = 0;
            partita->mappa2[i][j] = 0;
    /* setto la quantità massima di ogni pezzo a 20 */
    for (i = 0; i < 7; i++)
        partita->pezziRimasti[i] = 20;
    /* setto i punteggi dei giocatori entrambi a 0 */
    partita->punteggio1 = partita->punteggio2 = 0;
    partita->pezzoSelezionato = 0;
    return partita;
}

void incrementaPunteggio(int* punteggio, int lineeRimosse)
{
    /* aumento il punteggio del giocatore in funzione delle righe che ha rimosso*/
    switch (lineeRimosse)
    {
        case 1:  *punteggio+=1;  break;
        case 2:  *punteggio+=3;  break;
        case 3:  *punteggio+=6;  break;
        case 4:  *punteggio+=12; break;
    }
}

void richiediSceltaPezzo(Partita* partita)
{
    partita->pezzoSelezionato = 0;
    char cmd = '0';    /* variabile per memorizzare l'input */
    int scelto = 0;
    /* fino a che l'utente non preme invio o sceglie un pezzo non esaurito attendo l'input */
    while (cmd != '\n' || partita->pezziRimasti[scelto] <= 0)
    {
        Update(partita);
        cmd = getchar(); /* ottengo l'input */
        if(cmd == 'D') scelto = (scelto+1)%7;       /* se l'utente preme il la freccia destra o sinistra */
        if(cmd == 'C') scelto = (scelto+6)%7;       /* continuo a ciclare la lista di pezzi dispobiibli  */
        partita->pezzoSelezionato = scelto;
    }
    /* una volta che il pezzo è confermato diminuisco la quantità disponibile per lo stesso tipo */
    partita->pezziRimasti[scelto]--;
}

void cambiaTurno(Partita* partita)
{
    /* se è il turno del giocatore 2 passo al giocatore 1 */
    if(partita->turno == P2 || partita->Modalita == SINGLEPLAYER) 
        partita->turno = P1;
    else    /*altrimenti passo al giocatore 2*/
        partita->turno = P2;
}

void ScriviSuMappa(Partita* pa, Pezzo* pz, int valore)
{
    /* la dimensione del lato l del quadrato dell'area della matrice su cui devo scivere varia */
    /* in base al tipo del pezzo che uso come maschera */
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);
    /* uso un cursore per scorrere la matrice che rappresenta la disposizione del pezzo  */
    int *parser = pz->disposizione;
    for (i = 0; i < l*l; i++)
        if(*(parser++) == 1)    /* se in quel punto il pezzo esiste stampo sulla mappa il valore richiesto */
        {
            if(pa->turno == P1) pa->mappa1[pz->y + i/l][pz->x + i%l] = valore;  
            else                pa->mappa2[pz->y + i/l][pz->x + i%l] = valore;
        } 
    return;
}

Pezzo* generaPezzo(TipoPezzo tipo)
{
    /*  alloco memoria per la struct del nuovo pezzo */
    Pezzo *pz = malloc(sizeof(Pezzo));
    pz->tipo = tipo;
    pz->x = 3; pz->y = 0;   /* posiziono il nuovo pezzo in centro in alto alla mappa */
    assegnaDisposizione(pz);/* e gli assegno la dispozione in base al suo tipo */
    return pz;
}

void assegnaDisposizione(Pezzo* pz)
{
    /* la dimensione della disposizione di un pezzo dipende dal tipo di pezzo */
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
    pz->disposizione = malloc(sizeof(int)*l);                   /* alloco memoria per salvare la disposizione del pezzo nella sua struct */
    for(i = 0; i < l; i++) pz->disposizione[i] = schema[i];     /* e copio la disposizione standard del suo tipo nella sua struct*/
}

void distruggiPezzo(Pezzo* pz)
{
    /* libero la memoria occupata da un pezzo */
    free(pz->disposizione);
    free(pz);
}

void inserisciPezzo(Partita* pa, Pezzo* pz)
{
    /* scrivo sulla mappa il valore 2 che rappresenta un pezzo non confermato */
    /* passo il pezzo stesso come maschera per la stampa                      */
    ScriviSuMappa(pa,pz,2);
}

void rimuoviPezzo(Partita* pa, Pezzo* pz)
{
    /* scrivo sulla mappa il valore 0 che rappresenta uno spazio vuoto        */
    /* passo il pezzo stesso come maschera per la stampa                      */
    ScriviSuMappa(pa,pz,0);
}

void confermaPezzo(Partita* pa, Pezzo* pz)
{
    /* scrivo sulla mappa il valore 1 che rappresenta un pezzo confermato     */
    /* passo il pezzo stesso come maschera per la stampa                      */
    ScriviSuMappa(pa,pz,1);
}

void posizionaPezzo(Partita* partita, Pezzo* pz)
{
    char cmd = '0';/* variabile per memorizzare l'input */
    /* fino a che l'utente non preme invio continuo ad attendere l'input */
    while (cmd != '\n')
    {
        Update(partita); /* aggiorno la grafica perchè l'utente deve vedere il pezzo che ha appena mosso */
        cmd = getchar(); 
        int fatta = 0;
        if(cmd == 'A') fatta = muoviPezzo(partita,pz,0,-1);         /* questi sono i quattro casi che rappresentano */
        if(cmd == 'D') fatta = muoviPezzo(partita,pz,-1,0);         /* le quattro direzioni in qui un utente può    */
        if(cmd == 'B') fatta = muoviPezzo(partita,pz,0,1);          /* muovere ogni pezzo tramite le 4 freccette    */
        if(cmd == 'C') fatta = muoviPezzo(partita,pz,1,0);          /* direzionali                                  */
        if(cmd == '\n') 
            if(isPezzoPosizionabile(partita,pz))                    /* se l'utente preme invio controllo che il pezzo */
                confermaPezzo(partita,pz);                          /* sia posizionabile ed eventualmente lo confermo */
            else cmd = ' ';
        if(cmd == 'r') ruotaPezzo(partita, pz);                     /* se invece l'utente preme il tasto r ruoto il pezzo (sempre se possibile) */
    }
}

int muoviPezzo(Partita* pa, Pezzo* pz, int dx, int dy)
{
    rimuoviPezzo(pa,pz);                /* rimuovo il pezzo dalla mappa */
    pz->x += dx; pz->y += dy;           /* muovo il pezzo di dy e dx senza modificare la mappa */
    if(isPezzoSpostabile(pa,pz))        /* verifico se il pezzo in questa posizione è spostabile */
    {
        inserisciPezzo(pa,pz);          /* se è così, aggiorno la sua posizione anche nella mappa */
        return 1;
    }
    else
    {
        pz->x -= dx; pz->y -= dy;       /* altrimenti lo faccio tornare dove era */
        inserisciPezzo(pa,pz);          /* e lo inserisco nuovamente nella mappa */
        return 0;
    }
}

int ruotaPezzo(Partita* pa, Pezzo* pz)
{
    rimuoviPezzo(pa,pz); /* rimuovo il pezzo dalla mappa */

    int i,j, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);               /*                                             */
    int *result = malloc(sizeof(int)*l*l);                                  /* in questa sezione applico un algoritmo per  */
    int n = 0;                                                              /* ruotare la matrice che rappresenta la       */
    for(i = 0; i < l; i++)                                                  /* disposizione del blocco. E salvo la matrice */
    {                                                                       /* ruotata in una variabile a parte            */        
        for(j = l-1; j >= 0; j--)                                           /*                                             */
        {
            result[n++] = pz->disposizione[j*l + i];
        }
    }   
    int *old = pz->disposizione;                                            /* tuttavia non elimino la vecchia disposizione  */
    pz->disposizione = result;
    if(isPezzoSpostabile(pa,pz))                                            /* verifico se è possibile inserire il pezzo con */
    {                                                                       /* la nuova disposizione                         */
        free(old);                                                          /* se posso elimino la vecchia disposizione      */
        inserisciPezzo(pa,pz);
        return 1;
    }
    else
    {   
        pz->disposizione = old;                                             /* altrimenti annullo le modifiche fatte al pezzo */
        free(result);                                                       /* elimino la nuova disposizione                  */
        inserisciPezzo(pa,pz);                                              /* e inserisco di nuovo il pezzo                  */
        return 0;
    }
}

int isPezzoSpostabile(Partita* pa, Pezzo* pz)
{
    /* per verificare se il pezzo è spostabile in una data posizione devo scorrere la matrice rappresentante la       */
    /* sua disposizione sovrapponendola alla mappa di gioco per vedere se esce dai bordi o se collide con altri pezzi */
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3); /* ottengo la dimensione della dispozisione del pezzo */
    int result = 1;  /* variabile rappresentante la proprietà universale inizialmente vera*/
    /* scorro la disposizione del pezzo */
    for ( i = 0; i < l*l; i++)
    {
        int x = pz->x + i%l;                            /* ottengo le corrispondenti coordinate  */
        int y = pz->y + i/l;                            /* in relazione alla mappa               */

        if(x > -1 && y > -1 && x < 10 && y < 15)        /* verifico di essere entro i limiti della mappa di gioco */
        {
            /* osservo la mappa del giocatore che sta giocando */
            if(pa->turno == P1) result = !(*(pz->disposizione + i) && pa->mappa1[y][x]) && result;  /* verifico se in questa coordinata specifica */
            else                result = !(*(pz->disposizione + i) && pa->mappa2[y][x]) && result;  /* si verifica una collisione tra due pezzi   */
        }
        else if(*(pz->disposizione + i) && result)  /* controllo se il pezzo esiste fuori dai limiti della mappa */
            result = !*(pz->disposizione + i);      /* se ciò accade la mossa diventa illegale */
    }
    return result;
}

int isPezzoPosizionabile(Partita* pa, Pezzo* pz)
{
    /* per verificare se il pezzo è posizionabile ovvero confermabile devo scorrere la matrice rappresentante la */
    /* sua disposizione sovrapponendola alla mappa di gioco per vedere se è appoggiato a qualcosa o è fluttuante */
    int i, l = pz->tipo == O ? 2 : (pz->tipo == I ? 4 : 3);/* ottengo la dimensione della dispozisione del pezzo */
    int result = 0;/* variabile rappresentante la proprietà esistenziale inizialmente falsa */
    /* scorro la disposizione del pezzo */
    for ( i = 0; i < l*l; i++)
    {
        int x = pz->x + i%l;                            /* ottengo le corrispondenti coordinate  */
        int y = pz->y + i/l;                            /* in relazione alla mappa               */

        if(y == 14 && *(pz->disposizione + i)) result = 1;      /* se è appoggiato al fondo mappa ho già risolto */
        else if(x > -1 && y > -1 && x < 10 && y < 15)
        {
            /* osservo la mappa del giocatore che sta giocando */
            if(pa->turno == P1) result = (*(pz->disposizione + i) && pa->mappa1[y + 1][x] == 1) || result;      /* per ogni "blocco" che forma il pezzo */
            else                result = (*(pz->disposizione + i) && pa->mappa2[y + 1][x] == 1) || result;      /* guardo se sotto di esso c'è qualcosa */
        }
            
    }
    return result;
}

void invertiRighe(int mappa[15][10], int nrighe)
{
    int i = 0, j = 0;
    for (i = 14; i > 14-nrighe; i--)            /* scorro solo le righe richieste */
        for (j = 0; j < 10; j++)                /* scorro tutta la riga */
            mappa[i][j] = (mappa[i][j]+1)%2;    /* se il valore è 1 diventa 0 e viceversa */
    
}

int rimuoviLinee(Partita* pa)
{
    /* questa funzione ha lo scopo di verificare le righe che sono da rimuovere prima di rimuoverle     */
    /* lo fa contando quanti "1" sono presenti in una riga: se il valore è pari a 10 la riga va rimossa */
    int i,j;
    int rimosse = 0;
    for (i = 0; i < 15; i++)                                                /* passo ogni riga della matrice */
    {
        int count = 0;
        for(j = 0; j < 10; j++){                                            /* per ogni riga scorro le celle */
            if(pa->turno == P1) count = count + pa->mappa1[i][j];           /* e tengo il conto di quante    */
            else                count = count + pa->mappa2[i][j];           /* di queste hanno valore 1      */
        }
        if(count == 10)                                                     /* se la somma fa 10 la riga va rimossa */
        {
            for (j = 0; j < 10; j++){                                       /* scorro nuovamente la riga e   */
                if(pa->turno == P1) pa->mappa1[i][j] = 0;                   /* setto a zero tutti i valori   */
                else                pa->mappa2[i][j] = 0;
            }
            rimosse++;                                                      /* incremento il contatore righe rimosse */  
            abbassaBlocchi(pa,i);                                           /* abbasso i blocchi superiori alla riga tolta */
        }   
    }
    return rimosse;
}

void abbassaBlocchi(Partita* pa, int linea)
{
    int i,j;
    for (i = linea-1; i > -1; i--)                             /* scorro tutte le righe a partire da quella da elminiare   */
    {                                                          /* verso l'alto e le muovo in basso di una posizione ognuna */ 
        
        for (j = 0; j < 10; j++)                               /* scorro ogni cella di ogni riga */          
        {                                               
            if(pa->turno == P1)                                /* la mappa su cui lavoro dipende dal turno attuale */
            {
                pa->mappa1[i+1][j] = pa->mappa1[i][j];         /* copio la cella superiore in quella dove sono */
                pa->mappa1[i][j] = 0;                          /* setto la cella superiore pari a zero */
            }
            else
            {
                pa->mappa2[i+1][j] = pa->mappa2[i][j];          /* stesso procedimento per il giocatore 2 */
                pa->mappa2[i][j] = 0;                           
            }
        }
    }
}

int checkGameEnd(Partita* pa)
{
    /* la partita termina in 2 casi:   superato limite superiore    blocchi esauriti  */
    int i,j;
    for (i = 0; i < 2; i++)     /* scorro le due righe della mappa "vietate" */
    {
        for (j = 0; j < 10; j++)
        {
            if(pa->mappa1[i][j]) return 1;      /* se nella mappa1 la zona vietata è stata superata il giocatore 1 ha perso */
            if(pa->mappa2[i][j]) return 2;      /* se nella mappa2 la zona vietata è stata superata il giocatore 2 ha perso */
        }
    }
    int sum = 0;
    for (i = 0; i < 7; i++) 
        sum += pa->pezziRimasti[i];                         /* sommo tutti i pezzi rimasti insieme               */

    if(sum == 0)                                            /* verifico se sono uguali a zero                    */
        return pa->punteggio1 > pa->punteggio2 ? 2 : 1 ;    /* se questo accade vince il giocatore con più punti */

    return 0;                                               /* se niente di tutto ciò accade la partita continua */
}



