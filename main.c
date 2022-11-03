#include "headers.h"

int main() 
{
    /* ---------------------------------------------------------------------------------------- */
    /* questa sezione iniziale del codice utilizza metodi appartenenti alla libreria termios,   */
    /* per tanto, questa sezione è compatibile sono con i sitemi che adottano lo standard POSIX */
    /* lo scopo di questa parte di codice è quello di rendere l'input dell'utente istantaneo    */
    /* dopo la pressione di qualsiasi tasto evitando così di dover premere invio ogni volta     */

    struct termios info;
    tcgetattr(0, &info);          /* ottengo gli attributi del terminale; 0 rappresenta stdin   */
    info.c_lflag &= ~ICANON;      /* disattivo la modalità canonica                             */
    info.c_cc[VMIN] = 1;          /* aspetto che almeno un tasto sia premuto                    */
    info.c_cc[VTIME] = 0;         /* setto il timeout a zero                                    */
    tcsetattr(0, TCSANOW, &info); /* rendo effettivi i cambiamenti fatti sino ad ora            */

    /* ---------------------------------------------------------------------------------------- */

    while(1)
        menuIniziale();
        
    return 0;
}