#include <stdlib.h>
#include "socket.h"

int main(void) {

   int socketIP4 = socketCreation ();
    bindParam(socketIP4);
    binAdressPort(socketIP4);
    listening(socketIP4);
    connexion(socketIP4);

    return EXIT_SUCCESS;
}