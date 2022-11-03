#include "headers.h"

char ColoreDefault[] = "\033[0;0m";                   /* questi tre codici hanno lo scopo di cambiare il colore    */
char ColoreBianco[] = "\033[1;47m";                   /* allo sfondo del terminale così da poter disegnare blocchi */
char ColoreBlu[] = "\033[0;44m";                      /* e forme geometriche più pulite e belle da vedere          */

int CentroSchermo = 50;                               /* definita come la metà della larghezza del terminale, la variabile */
                                                      /* CentroSchermo viene aggiornata sempre prima di stampare a schermo */

int I_PATTERN[16] = {0,0,0,0,
                     1,1,1,1,
                     0,0,0,0,
                     0,0,0,0};

int J_PATTERN[9] = {1,0,0,
                    1,1,1,
                    0,0,0}; 

int L_PATTERN[9] = {0,0,1,                            /* questi sono i pattern di default per i vari pezzi.       */
                    1,1,1,                            /* quando un pezzo viene creato il relativo pattern         */
                    0,0,0};                           /* viene copiato al suo interno e alterato successivamente  */

int O_PATTERN[4] = {1,1,
                    1,1}; 

int S_PATTERN[9] = {0,1,1,
                    1,1,0,
                    0,0,0}; 

int T_PATTERN[9] = {0,1,0,
                    1,1,1,
                    0,0,0}; 

int Z_PATTERN[9] = {1,1,0,
                    0,1,1,
                    0,0,0}; 