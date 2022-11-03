#include "headers.h"

int bestX = 0, bestY = 1, bestR = 0, bestP = -1000;
int punteggioBest = 0;

void eseguiMossaCPU(Partita* pa)
{
    miglioreMossa(pa);                                     /* prima di tutto calcolo la migliore mossa                   */  

    Pezzo *pz = generaPezzo(bestP);                        /* genero il pezzo in base al migliore tipo                   */
    while (bestR-- > 0)                                         
        ruotaPezzo(pa,pz);                                 /* ruoto il pezzo fino a raggiungere la migliore rotazione    */

    rimuoviPezzo(pa,pz);                                   /* rimuovo il pezzo dalla mappa                               */
    pz->x = bestX;                                         /* lo riposiziono alle                                        */
    pz->y = bestY;                                         /* coordinate corrette                                        */
    inserisciPezzo(pa,pz);                                 /* lo re-inserisco nella mappa                                */
    confermaPezzo(pa,pz);                                  /* e confermo la sua posizione                                */
    
    int rimosse = rimuoviLinee(pa);                         /* verifico se ci sono righe da eliminare e salvo la quantità */

    incrementaPunteggio(&pa->punteggio2, rimosse);         /* incremento il punteggio in base alle righe rimosse         */
    if(rimosse >= 3 )                                      /* se il numero di righe rimosse è uguale o maggiore di tre   */
        invertiRighe( pa->mappa1, rimosse);                /* inverto le righe del giocatore                             */

    pa->pezziRimasti[bestP]--;                             /* diminuisco il numero di pezzi disponibile del tipo scelto  */

    punteggioBest = 0;
    
    return;
}

int miglioreMossa(Partita* pa)
{
    pa->pezzoSelezionato = S;                                       /* in questa funzione testo ogni mossa possibile per determinare la    */
    Pezzo *pz;                                                      /* migliore in ogni data situazione                                    */
    int tipo , rot;
    for (tipo = 0; tipo <= Z; tipo++ )                              /* il for più esterno ha il compito di scorrere tutti i tipi di pezzo  */
    {
        if(pa->pezziRimasti[tipo] > 0)                              /* verifico che i pezzi di questo tipo non siano finiti                */
        {
            pz = generaPezzo(tipo);                                 /* genero il pezzo                                                     */
            muoviPezzo(pa,pz,0,0);                                  /* e lo posiziono nella mappa                                          */
            
            for (rot = 0; rot < 4; rot++)                           /* con questo ciclo testo tutte le 4 rotazioni possibili del pezzo     */
            {
                while (muoviPezzo(pa,pz,-1,0));                     
                do                                                  /* con questo ciclo testo tutte le posizioni nell'asse delle x         */
                {
                    while (muoviPezzo(pa,pz,0,1));                  /* fino a che non incontro qualcosa continuo a muoverlo verso il basso */
                    int punteggio = calcolaPunteggioScenario(pa);   /* calcolo il punteggio dello scenario che si è venuto a creare        */
                    if(punteggio > punteggioBest)
                    {
                        bestX = pz->x;                              /* se il punteggio appena calcolato risulta maggiore del punteggio     */
                        bestY = pz->y;                              /* best, salvo in memoria il nuovo punteggio e i dati relativi alla    */
                        bestR = rot;                                /* mossa come: tipo pezzo, rotazione e coordinate                      */
                        bestP = tipo;
                        punteggioBest = punteggio;
                    }

                    while (pz->y != 1)
                        muoviPezzo(pa,pz,0,-1);                     /* resetto il pezzo alla posizione di partenza per essere ruotato      */
                } while (muoviPezzo(pa,pz,1,0));

                while(!ruotaPezzo(pa,pz))                           /* muovo il pezzo a sinistra fino a che non riesco a passare alla      */
                    muoviPezzo(pa,pz,-1,0);                         /* prossima rotazione                                                  */
            }
            rimuoviPezzo(pa,pz);                                    /* tolgo il pezzo dalla mappa                                          */
            distruggiPezzo(pz);                                     /* libero la memoria occupada dalla struct del pezzo                   */
        }
    }
}

int calcolaPunteggioScenario(Partita* pa)
{
    int i,j;
    int trovato = 0;
    int punteggio = 100;                                                    /* ogni scenario inizia con un punteggio base di 100 punti      */
    for (i = 0; i < 15 && !trovato; i++)                                
    {                                                                       /* scorro tutte le righe per vedere se sono occupate da blocchi */
        for ( j = 0; j < 10; j++)                                           
        {
            trovato = pa->mappa2[i][j] + trovato;                           /* passo tutte le colonne per vedere se trovo almeno un blocco  */
        }                                                                   
    }
    punteggio += (16-i)*(-1);                                               /* la variabile punteggio viene decrementata in base a quanto   */
    int n;                                                                  /* alta è la costruzione di blocchi costruita                   */
    for ( n = 14; n >= i-1; n--)
    {                                                                       /* ciclo tutte le righe occupate da almeno un blocco            */
        for ( j = 0; j < 10; j++)           
        {                                                                   /* per ogni riga cerco quanti "buchi" sono presenti             */
            if(pa->mappa2[n][j] == 0)
                punteggio = punteggio - 3 -2*pa->mappa2[n-1][j];            /* rimuovo dal punteggio 3 punti per ogni buco che trovo        */
        }                                                                   /* e tolgo 2 punti ulteriori se il blocco non è raggiungibile   */
    }
    return punteggio;
}