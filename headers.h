#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// ------------------------------------------------------------------------------------//
//                        DICHIARAZIONE DELLE VARIABILI LOCALI                         //
// ------------------------------------------------------------------------------------//

/**colore dello sfondo*/
extern char ColoreDefault[];
/**definizione del colore 'rosso'*/
extern char ColoreRosso[];
/**definizione del colore 'blu'*/
extern char ColoreBlu[];

/**centro dello schermo in caratteri di distanza del margine destro della console*/
extern int CentroSchermo;   

// ------------------------------------------------------------------------------------//
//                  DICHIARAZIONE DELLE STRUCT E DEGLI ENUMERATORS                     //
// ------------------------------------------------------------------------------------//


/*  ENUMERATOR per gestire i due giocatori*/
typedef enum {P1,P2} Giocatore;

/*  ENUMERATOR per gestire tipi di pezzi*/
typedef enum {I,T,O,L,J,S,Z} TipoPezzo;

/* ENUMERATOR per gestire la modalità di gioco*/
typedef enum {SINGLEPLAYER,MULTIPLAYER} Modalita;

/* ENUMMERATOR per memorizzare lo stato della partita*/
typedef enum {IN_CORSO,TERMINATA,VITTORIA_P1,VITTORIA_P2} Stato;

/* ENUMMERATOR per memorizzare la rotazione di un pezzo*/
typedef enum {UNO,DUE,TRE,QUATTRO} Rotazione;

typedef struct
{
    Modalita Modalita;              ///< modalità di gioco selszionata
    Giocatore turno;                ///< turno corrente
    Stato stato;                    ///< stato della partita

    int mappa[15][10];              ///< mappa di 15 righe e 10 colonne
} Partita;

typedef struct
{
    TipoPezzo tipo;                 ///< indica quale dei 7 pezzi è
    Rotazione rotazione;            ///< indica quanto è ruotato

    int *disposizione;              ///< la disposizione del pezzo in una griglia 2x2 3x3 4x4
} Pezzo;

// ------------------------------------------------------------------------------------//
//                            DICHIARAZIONE DELLE FUNZIONI                             //
// ------------------------------------------------------------------------------------//

/**
*uno strlen più avanzato che tiene conto dei caratteri speciali in utf8
*@param *s array di caratteri da contare
*/
int StrLenMB(char *s);

/**
*funzione di stampa che allinea la stringa da stampare con il resto del programma
*@param *s array di caratteri da stampare
*/
void Stampa(char *s);

/**
*simile a funzione Stampa ma tiene conto di caratteri speciali per il colore
*@param *s array di caratteri da stampare
*@param NumColoriUsati quantità dei codici colore usati
*/
void StampaConColore(char *s, int NumColoriUsati);

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