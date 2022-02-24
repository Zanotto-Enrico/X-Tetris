#include "headers.h"

char ColoreDefault[] = "\033[0;0m";
char ColoreRosso[] = "\033[1;47m";
char ColoreBlu[] = "\033[0;44m";

int CentroSchermo = 50;

int I_PATTERN[4][4] = {{0,0,0,0},
                       {1,1,1,1},
                       {0,0,0,0},
                       {0,0,0,0}};

int J_PATTERN[3][3] = {{1,0,0},
                       {1,1,1},
                       {0,0,0}}; 

int L_PATTERN[4][4] = {{0,0,1},
                       {1,1,1},
                       {0,0,0}}; 

int O_PATTERN[2][2] = {{1,1},
                       {1,1}}; 

int S_PATTERN[4][4] = {{0,1,1},
                       {1,1,0},
                       {0,0,0}}; 

int T_PATTERN[4][4] = {{0,1,0},
                       {1,1,1},
                       {0,0,0}}; 

int Z_PATTERN[4][4] = {{1,1,0},
                       {0,1,1},
                       {0,0,0}}; 