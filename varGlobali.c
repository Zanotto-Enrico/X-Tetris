#include "headers.h"

char ColoreDefault[] = "\033[0;0m";
char ColoreBianco[] = "\033[1;47m";
char ColoreBlu[] = "\033[0;44m";

int CentroSchermo = 50;

int I_PATTERN[16] = {0,0,0,0,
                     1,1,1,1,
                     0,0,0,0,
                     0,0,0,0};

int J_PATTERN[9] = {1,0,0,
                    1,1,1,
                    0,0,0}; 

int L_PATTERN[9] = {0,0,1,
                    1,1,1,
                    0,0,0}; 

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