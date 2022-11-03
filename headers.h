#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>

/* ------------------------------------------------------------------------------------*/
/*                        DICHIARAZIONE DELLE VARIABILI GLOBALI                        */
/* ------------------------------------------------------------------------------------*/

/** colore dello sfondo*/
extern char ColoreDefault[];
/** definizione del colore 'bianco'*/
extern char ColoreBianco[];
/** definizione del colore 'blu'*/
extern char ColoreBlu[];

/** centro dello schermo in caratteri di distanza del margine destro della console*/
extern int CentroSchermo;   

/** pattern di default per un blocco di tipo I */
extern int I_PATTERN[16];
/** pattern di default per un blocco di tipo J */
extern int J_PATTERN[9]; 
/** pattern di default per un blocco di tipo L */
extern int L_PATTERN[9]; 
/** pattern di default per un blocco di tipo O */
extern int O_PATTERN[4]; 
/** pattern di default per un blocco di tipo S */
extern int S_PATTERN[9]; 
/** pattern di default per un blocco di tipo T */
extern int T_PATTERN[9]; 
/** pattern di default per un blocco di tipo Z */
extern int Z_PATTERN[9]; 

/* ------------------------------------------------------------------------------------*/
/*                  DICHIARAZIONE DELLE STRUCT E DEGLI ENUMERATORS                     */
/* ------------------------------------------------------------------------------------*/


/**  ENUMERATOR per gestire i due giocatori*/
typedef enum {P1,P2} Giocatore;

/**  ENUMERATOR per gestire tipi di pezzi*/
typedef enum {I,T,O,L,J,S,Z} TipoPezzo;

/** ENUMERATOR per gestire la modalità di gioco*/
typedef enum {SINGLEPLAYER,MULTIPLAYER,CPU} Modalita;

/** ENUMMERATOR per memorizzare lo stato della partita*/
typedef enum {IN_CORSO,TERMINATA,VITTORIA_P1,VITTORIA_P2} Stato;

/**
 * Struct contenente tutte le principali informazioni per lo svolgimento di una partita. 
 * Informaizoni come il turno attuale, lo stato della partita, i punteggi dei giocatori,
 * i blocchi rimasti e anche le matrici rappresentanti i campi da gioco.
 * Questa stract viene inizializzata all'inizio di ogni nuova partita e viene eliminata solo
 * al termine della stessa
 */
typedef struct
{
    Modalita Modalita;              /**< modalità di gioco selszionata*/
    Giocatore turno;                /**< turno corrente*/
    Stato stato;                    /**< stato della partita*/

    TipoPezzo pezzoSelezionato;     /**< il tipo del pezzo selezionato*/

    int punteggio1;                 /**< punteggio giocatore uno*/
    int punteggio2;                 /**< punteggio giocatore due*/

    int pezziRimasti[7];            /**< pezzi ancora disponibili*/

    int mappa1[15][10];              /**< mappa di 15 righe e 10 colonne*/
    int mappa2[15][10];              /**< mappa di 15 righe e 10 colonne*/
} Partita;

/**
 * Struct contenente informazioni riguardanti un tipo di pezzo che il giocatore sta manipolando.
 * Essa contiene dati come la posizione attuale del pezzo nella mappa, il tipo del pezzo e un puntatore
 * alla sua attuale disposizione. Quest'ultima è una matrice rappresentante forma e rotazione attuale del 
 * pezzo, che nel momento della creazione, viene copiata dal pattern default nelle variabili statiche.
 * Tuttavia durante la paritta il giocatore può alterarla utilizzando la funzione di rotazione
 */
typedef struct
{
    TipoPezzo tipo;                 /**< indica quale dei 7 pezzi è*/
    int x;                          /**< coordinata x della posizione */
    int y;                          /**< coordinata y della posizione*/

    int *disposizione;              /**< la disposizione del pezzo in una griglia 2x2 3x3 4x4*/
} Pezzo;

/* ------------------------------------------------------------------------------------*/
/*                            DICHIARAZIONE DELLE FUNZIONI                             */
/* ------------------------------------------------------------------------------------*/

                            /* --------------------------- */
                            /*      FUNZIONI GRAFICHE      */
                            /* --------------------------- */

/**
*Funzione principale per l'aggiornamento della grafica, esegue un refresh dello
*stato della partita sul terminale invocando tutte le funzioni grafiche necessarie.
*Funzine da invocare qualora si facciano modifiche alla struct Partita 
*@param *partita puntatore alla struct partita
*/
void Update(Partita *partita);   

/**
*posiziona il cursore in una precisa posizione nel terminale. Funzione fondamentale
*nel render della partita poichè permette di separare la stampa di vari elementi della
*schermata in più funzioni diverse.
*@param x coordinata x
*@param y coordinata y
*/
void gotoxy(int x,int y);

/**
*aggiorna la variabile statica CentroSchermo se rileva che il terminale
*ha cambiato dimensione
*/
void aggiornaCentroSchermo();

/**
*uno strlen più avanzato che tiene conto dei caratteri speciali in utf8,
*funzione necessaria allo scopo di utilizzare stringhe colorate nel terminale
*@param s array di caratteri da contare
*/
int StrLenMB(char *s);

/**
*funzione di stampa che allinea la stringa da stampare con il resto del programma
*senza ritorno a capo
*@param s array di caratteri da stampare
*/
void Stampa(char *s);

/**
*funzione di stampa che allinea la stringa da stampare con il resto del programma
*con ritorno a capo
*@param s array di caratteri da stampare
*/
void StampaL(char *s);

/**
*simile a funzione Stampa ma tiene conto di caratteri speciali per il colore
*@param s array di caratteri da stampare
*@param NumColoriUsati quantità dei codici colore usati
*/
void StampaLConColore(char *s, int NumColoriUsati);

/**
 *funzione con lo scopo di "pulire" il terminale stampando stringhe di spazi lunghi 
 *quanto la larghezza del terminale stesso.
 */
void pulisciSchermo();

/**
*stampa in ASCII ART il titiolo "X-Tetris"
*/
void StampaTitolo();

/**
*stampa in il menù iniziale per la scelta della modalità con la relativa legenda
*per l'input utente
*@param mode enumerator rappresentante la modalità attualmente selezionata
*/
void StampaMenuIniziale(Modalita mode);

/**
*stampa la Modalità di gioco corrente 
*@param partita puntatore alla struct partita
*/
void StampaModalita(Partita *partita);

/**
*stampa info riguardanti chi ha il turno 
*@param g puntatore al giocatore che ha il turno
*@param x posizione orizzontale nel terminale su cui stampare
*/
void StampaTurno(Giocatore* g, int x);

/**
*stampa info riguardanti il punteggio attuale dei giocatori
*@param pa puntatore alla struct partita
*@param x posizione orizzontale nel terminale su cui stampare
*/
void StampaPunteggio(Partita* pa, int x);

/**
*stampa a video a fianco del campo di gioco il menu che permette all'utente 
*di vedere quale pezzo sta per selezionare con la relativa legenda per l'input
*@param pz puntatore al tipo di pezzo attualmente selezionato
*@param int[7] array contenente la quantità rimasta per pezzo
*@param x posizione orizzontale nel terminale su cui stampare
*/
void StampaMenuSceltaPezzo(TipoPezzo pz, int pezziRimasti[7], int x);

/**
*funzione utilizzata nella stampa dell'effettivo campo di gioco. Viene chiamata dalla funzione
*Update una volta in singleplayer o due in multiplayer per permettere ai giocatori di vedere lo
*stato del proprio campo di gioco e di quello dell'avversario contemporaneamente
*@param int[15][10] matrice rappresentante il campo da gioco
*@param x posizione orizzontale nel terminale su cui stampare
*@param y posizione verticale nel terminale su cui stampare
*/
void stampaMappa(int mappa1[15][10], int x, int y);

/**
*stampa il resoconto di fine partita comprendente i punteggi dei giocatori e 
*info riguardanti il vincitore.
*@param pa puntatore alla struct partita
*@param x posizione orizzontale nel terminale su cui stampare
*/
void stampaFine(Partita *pa, int x);



                            /* --------------------------- */
                            /*        EVENTI PARTITA       */
                            /* --------------------------- */

/**
*Attende l'input dell'utente e richiama la funzione di stampa menu iniziale ogni
*volta che l'utente effettua un cambiamento nella selezione della modalità
*/
void menuIniziale();

/**
*Funzione principale che viene chiamata all'inizio di una nuova partita e che termina con la
*conclusione della stessa. Si occuppa di chiamare tutte le funzioni per inizializzare e 
*e gestire lo svolgimento dell'intera partita
*@param mode modalità scelta per la nuova partita
*/
void IniziaPartita(Modalita mode);

/**
*Funzione con lo scopo di allocare memoria per una nuova struct partita e inizializzare le varie 
*strutture dati presenti in essa, contenenti informazioni come la mappa di gioco e l'array di pezzi rimasti
*@param mode modalità scelta per la nuova partita
*@retval Partita* pointer alla struct Partita appena creata 
*/
Partita* inizializzaPartita(Modalita mode);

/**
*Attende l'input utente riguardo alla scelta del pezzo e aggiorna lo schermo ogni volta che 
*la scelta cambia. Premendo invio la funzione conferma la selezione e decremena i pezzi rimanenti
*di quello specifico tipo
*@param partita puntatore alla struct partita
*/
void richiediSceltaPezzo(Partita* partita);

/**
*Incrementa il punteggio di un giocatore in funzione di quante righe ha rimosso in questo turno.
* 1 riga -> 1 punto
* 2 righe -> 3 punti
* 3 righe -> 6 punti
* 4 righe -> 12 punti
*@param int puntatore alla var contenente il punteggio del giocatore
*@param int righe rimosse dal giocatore in questo turno 
*/
void incrementaPunteggio(int* punteggio, int lineeRimosse);

/**
*Funzione con il semplice scopo di passare il turno all'altro giocatore aggiornando la
*relativa variabile nella struct partita
*@param partita puntatore alla struct partita
*/
void cambiaTurno(Partita* partita);


/**
*Funzione con il compito di inserire un pezzo nella matrice rappresenante la mappa .
*In base allo stato del pezzo (confermato o non confermato) il valore con la quale il pezzo
*viene inserito nella mappa cambia.
*@param pa puntatore alla struct partita
*@param pz pezzo rappresentante l'area e la posizione di scrittura
*@param valore stato del pezzo che si sta inserendo  (1 = confermato, 2 = non confermato, 0 = spazio vuoto)
*/
void ScriviSuMappa(Partita* pa, Pezzo* pz, int valore);

/**
*Funzione con lo scopo di allocare memoria per una nuova struct pezzo e assegnargli come
*disposizione una delle matrici globali rappresentanti i vari pattern
*@param tipo enumerator rappresentante il tipo di pezzo che di vuole generare
*@retval Pezzo* pointer alla struct Pezzo appena creata 
*/
Pezzo* generaPezzo(TipoPezzo tipo);

/**
*Funzione con lo scopo di copiare una delle matrici globali nella matrice di disposzione del pezzo
*La diposizione di interi nella matrice rappresenta il pattern e la forma
*del pezzo stesso nel momento in cui dovrà essere stampato nella mappa
*@param pz pointer alla struct Pezzo da aggionrnare
*/
void assegnaDisposizione(Pezzo* pz);

/**
*Funzione con lo scopo di liberare la memoria occupata da una struct pezzo non più necessaria.
*Viene liberata anche la matrice dispozine contenuta nella struct
*@param pz pointer alla struct Pezzo da eliminare 
*/
void distruggiPezzo(Pezzo* pz);

/**
*Funzione con lo scopo di inserire nella mappa un pezzo non ancora confermato.
*Ciò vinene fatto invocando la funzione ScriviSuMappa sul pezzo pz con valore 2
*@param pa puntatore alla struct partita
*@param pz pointer al pezzo da inserire
*/
void inserisciPezzo(Partita* pa, Pezzo* pz);

/**
*Funzione con lo scopo di rimuovere dalla mappa un pezzo.
*Ciò vinene fatto invocando la funzione ScriviSuMappa sul pezzo pz con valore 0
*@param pa puntatore alla struct partita
*@param pz pointer al pezzo da inserire
*/
void rimuoviPezzo(Partita* pa, Pezzo* pz);

/**
*Funzione con lo scopo di confermare nella mappa un pezzo non ancora confermato.
*Ciò vinene fatto invocando la funzione ScriviSuMappa sul pezzo pz con valore 1
*@param pa puntatore alla struct partita
*@param pz pointer al pezzo da inserire
*/
void confermaPezzo(Partita* pa, Pezzo* pz);

/**
*Attende l'input dell'utente per muovere un pezzo non ancora confermato.
*Ogni volta che l'utente preme una freccetta direzionale viene invocata la funzione muoviPezzo
*che proverà, se possibile, a muovere il pezzo nella direzione indicata.
*Se l'utente preme invio invece il pezzo viene confermato (sempre se possibile)
*@param pa puntatore alla struct partita
*@param pz pointer al pezzo non confermato da muovere
*/
void posizionaPezzo(Partita* partita, Pezzo* pz);

/**
*Funzione con lo scopo di muovere un pezzo presente nella mappa di un dato delta x e delta y
*rispetto alla sua attuale posizione. Prima di fare ciò si accerta che l'operazione sia legale
*invocando la funzione isPezzoSpostabile
*@param pa puntatore alla struct partita
*@param pz pointer al pezzo non confermato da muovere
*@param dx delta x
*@param dy delta y
*/
int muoviPezzo(Partita* pa, Pezzo* pz, int dx, int dy);

/**
*Funzione con lo scopo di ruotare un pezzo presente nella mappa in senso orario di 90 gradi.
*Prima di fare ciò si accerta che l'operazione sia legale invocando la funzione isPezzoSpostabile
*@param pa puntatore alla struct partita
*@param pz pointer al pezzo non confermato da muovere
*/
int ruotaPezzo(Partita* pa, Pezzo* pz);

/**
*Funzione con lo scopo di verificare se un pezzo NON ancora confermato è spostabile nella
*posizione indicata dalle sue coordinate presenti all'interno della sua struct.
*Se il pezzo si sovrappone ad altri pezzi o è fuori dalla mappa il pezzo non è inseribile
*@param pa puntatore alla struct partita
*@param pz pointer alla struct del pezzo 
*@retval 1 se il pezzo è spostabile
*@retval 0 se il pezzo non è spostabile
*/
int isPezzoSpostabile(Partita* pa, Pezzo* pz);

/**
*Funzione con lo scopo di verificare se è possibile confermare un pezzo nella posizione in cui si trova.
*Perchè ciò sia possibile il pezzo deve essere "appoggiato" su un altro blocco o sul fondo mappa
*@param pa puntatore alla struct partita
*@param pz pointer alla struct del pezzo
*@retval 1 se il pezzo è confermabile
*@retval 0 se il pezzo non è confermabile
*/
int isPezzoPosizionabile(Partita* pa, Pezzo* pz);

/**
*Funzione con lo scopo di invertire n righe in una data mappa a partire dal findo mappa. 
*Viene invocata dopo che un giocatore ha eliminato 3 o più righe dal suo campo 
*@param mappa[15][10] mappa da invertire
*@param nrighe numero di righe da eliminare
*/
void invertiRighe(int mappa[15][10], int nrighe);

/**
*Funzione creata per scorrere le righe della mappa eliminando quelle che sono complete.
*Ogni volta che ciò accade invoca abbassaBlocchi per abbassare i blocchi al di sopra della linea.
*Ritorna in oltre il numero di righe eliminate necessarie alla modifica dei punteggi
*@param pa puntatore alla struct partita
*@retval int rappresentante il numero di righe eliminate
*/
int rimuoviLinee(Partita* pa);


/**
*Funzione con lo scopo di abbassare righe di blocchi, in genere invocata dopo che una
*o più righe sono state eliminate
*@param pa puntatore alla struct partita
*@param linea altezza dell'ultima riga da abbassare
*/
void abbassaBlocchi(Partita* pa, int linea);


/**
*Verifica se la partita è giunta al termine o meno e determina il vincitore. Invocata 
*in genere dopo ogni turno. La partita termina se un giocatore supera il limite superiore
*o TUTTI i blocchi finiscono
*@param pa puntatore alla struct partita
*@retval 0 se la partita non è ancora terminata 
*@retval 1 se il giocatore 1 ha perso
*@retval 2 se il giocatore 2 ha perso
*/
int checkGameEnd(Partita* pa);

                            /* --------------------------- */
                            /*         FUNZIONI CPU        */
                            /* --------------------------- */
/**
 * Funzione da invocare per far eseguire una mossa alla macchina. Questa funzione si occupa
 * non solo di invocare la funzione miglioreMossa ma anche di generare il pezzo, inserirlo
 * nella posizione corretta, rimuovere le eventuali righe e aggioarnare il punteggio tramite
 * chiamate ad apposite funzioni
 * @param pa 
 */
void eseguiMossaCPU(Partita* pa);


int miglioreMossa(Partita* pa);

/**
 * Funzione creata con lo scopo di calcolare il punteggio di uno scenario basato sulla
 * disposizione dei pezzi all'interno della mappa, un punteggio maggiore è indice di
 * uno scenario "buono" per la CPU.
 * Ogni scenario inizia da un punteggio pari a 100 che viene decrementato da 2 fattori:
 * 1) viene tolto una quantità di punti direttamente proporzionale all'altezza raggiunta dai blocchi
 * 2) viene tolto una quantità di punti direttamente proporzionale alla quantità di "buchi" presenti
 *    sui livelli già occupati
 * @param pa 
 */
int calcolaPunteggioScenario(Partita* pa);

