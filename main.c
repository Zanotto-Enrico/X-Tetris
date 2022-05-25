#include "headers.h"

int main() 
{

    struct termios info;
    tcgetattr(0, &info);          /* ottengo gli attributi del terminale; 0 rappresenta stdin */
    info.c_lflag &= ~ICANON;      /* disattivo la modalità canonica */
    info.c_cc[VMIN] = 1;          /* aspetto che almeno un tasto sia premuto */
    info.c_cc[VTIME] = 0;         /* setto il timeout a zero */
    tcsetattr(0, TCSANOW, &info); /* setto gli attributi */


    menuIniziale();
    return 0;
}