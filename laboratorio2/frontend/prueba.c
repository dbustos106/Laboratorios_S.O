#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "solicitud.h"

int main(int argc, char *argv[]){  
    int clientfd = hacerConexion(argv[1]);
    int sourceId = 32;
    enviarDatos(clientfd, "sourceid", sourceId);
}