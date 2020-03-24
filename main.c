#include <stdlib.h>
#include "sockets/creation/creation.h"
#include "sockets/Parameters/bindParam.h"
#include "sockets/Adress-Ports/bindAdressPort.h"
#include "sockets/listening/listening.h"
#include "sockets/connexion/connexion.h"


int main(void) {

   int socketIP4 = socketCreation ();
    bindParam(socketIP4);
    binAdressPort(socketIP4);
    listening(socketIP4);
    connexion(socketIP4);

    return EXIT_SUCCESS;
}