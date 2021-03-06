#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>

/* ------------------------------------------------------------------------------------*/
/*                        DICHIARAZIONE DELLE VARIABILI LOCALI                         */
/* ------------------------------------------------------------------------------------*/

/**colore dello sfondo*/
extern char ColoreDefault[];
/**definizione del colore 'bianco'*/
extern char ColoreBianco[];
/**definizione del colore 'blu'*/
extern char ColoreBlu[];

/**centro dello schermo in caratteri di distanza del margine destro della console*/
extern int CentroSchermo;   

extern int I_PATTERN[16];

extern int J_PATTERN[9]; 

extern int L_PATTERN[9]; 

extern int O_PATTERN[4]; 

extern int S_PATTERN[9]; 

extern int T_PATTERN[9]; 

extern int Z_PATTERN[9]; 

/* ------------------------------------------------------------------------------------*/
/*                  DICHIARAZIONE DELLE STRUCT E DEGLI ENUMERATORS                     */
/* ------------------------------------------------------------------------------------*/


/*  ENUMERATOR per gestire i due giocatori*/
typedef enum {P1,P2} Giocatore;

/*  ENUMERATOR per gestire tipi di pezzi*/
typedef enum {I,T,O,L,J,S,Z} TipoPezzo;

/* ENUMERATOR per gestire la modalità di gioco*/
typedef enum {SINGLEPLAYER,MULTIPLAYER,CPU} Modalita;

/* ENUMMERATOR per memorizzare lo stato della partita*/
typedef enum {IN_CORSO,TERMINATA,VITTORIA_P1,VITTORIA_P2} Stato;

/* ENUMMERATOR per memorizzare la rotazione di un pezzo*/
typedef enum {UNO,DUE,TRE,QUATTRO} Rotazione;

typedef struct
{
    Modalita Modalita;              /*< modalità di gioco selszionata*/
    Giocatore turno;                /*< turno corrente*/
    Stato stato;                    /*< stato della partita*/

    TipoPezzo pezzoSelezionato;     /*< il tipo del pezzo selezionato*/

    int punteggio1;                 /*< punteggio giocatore uno*/
    int punteggio2;                 /*< punteggio giocatore due*/

    int pezziRimasti[7];            /*< pezzi ancora disponibili*/

    int mappa1[15][10];              /*< mappa di 15 righe e 10 colonne*/
    int mappa2[15][10];              /*< mappa di 15 righe e 10 colonne*/
} Partita;

typedef struct
{
    TipoPezzo tipo;                 /*< indica quale dei 7 pezzi è*/
    int x;                          /*< coordinata x della posizione */
    int y;                          /*< coordinata y della posizione*/

    int *disposizione;              /*< la disposizione del pezzo in una griglia 2x2 3x3 4x4*/
} Pezzo;

/* ------------------------------------------------------------------------------------*/
/*                            DICHIARAZIONE DELLE FUNZIONI                             */
/* ------------------------------------------------------------------------------------*/

/**
*uno strlen più avanzato che tiene conto dei caratteri speciali in utf8
*@param *s array di caratteri da contare
*/
int StrLenMB(char *s);

/**
*funzione di stampa che allinea la stringa da stampare con il resto del programma
*senza ritorno a capo
*@param *s array di caratteri da stampare
*/
void Stampa(char *s);

/**
*funzione di stampa che allinea la stringa da stampare con il resto del programma
*con ritorno a capo
*@param *s array di caratteri da stampare
*/
void StampaL(char *s);

/**
*simile a funzione Stampa ma tiene conto di caratteri speciali per il colore
*@param *s array di caratteri da stampare
*@param NumColoriUsati quantità dei codici colore usati
*/
void StampaLConColore(char *s, int NumColoriUsati);

/**
*stampa in ASCII ART il titiolo "X-Tetris"
*/
void StampaTitolo();

/**
*stampa la Modalita corrente
*@param *partita puntatore alla struct partita
*/
void StampaModalita(Partita *partita);

/**
*aggiorna la mappa a video, funzione da chiamare se sono 
*state fatte modifiche
*@param *partita puntatore alla struct partita
*/
void Update(Partita *partita);   

void inserisciPezzo(Partita* pa, Pezzo* pz);

void rimuoviPezzo(Partita* pa, Pezzo* pz);

int isPezzoSpostabile(Partita* pa, Pezzo* pz);

int isPezzoPosizionabile(Partita* pa, Pezzo* pz);

int muoviPezzo(Partita* pa, Pezzo* pz, int dx, int dy);

void confermaPezzo(Partita* pa, Pezzo* pz);

void invertiRighe(Partita* pa, int mappa[15][10], int nrighe);

/**
*posiziona il cursore in una precisa posizione nel terminale
*@param int x coordinata x
*@param int y coordinata y
*/
void gotoxy(int x,int y);

/**
*aggiorna la variabile pubblica CentroSchermo se il terminale
*ha cambiato dimensione
*/
void aggiornaCentroSchermo();

Partita* inizializzaPartita(Modalita mode);

void incrementaPunteggio(int* punteggio, int lineeRimosse);

void posizionaPezzo(Partita* partita, Pezzo* pz);

void abbassaBlocchi(Partita* pa, int linea);

void assegnaDisposizione(Pezzo* pz);

void richiediSceltaPezzo(Partita* partita);

void cambiaTurno(Partita* partita);

Pezzo* generaPezzo(TipoPezzo tipo);

int ruotaPezzo(Partita* pa, Pezzo* pz);

void distruggiPezzo(Pezzo* pz);

int rimuoviLinee(Partita* pa);

int checkIfLost(Partita* pa);

void StampaMenuIniziale(Modalita mode);

void menuIniziale();

void StampaTurno(Giocatore* g);

void StampaMenuSceltaPezzo(TipoPezzo pz, int pezziRimasti[7]);

void StampaPunteggio(Partita* pa);

void stampaMappa(int mappa1[15][10], int x, int y);

void ScriviSuMappa(Partita* pa, Pezzo* pz, int valore);