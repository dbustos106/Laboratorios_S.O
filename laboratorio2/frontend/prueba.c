#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "solicitud.h"

int main(int argc, char *argv[]){  
    int clientfd = hacerConexion(argv[1]);
    int sourceId = 584;
    enviarDatos(clientfd, "sourceid", sourceId);
    int dstId = 208;
    enviarDatos(clientfd, "dstid", dstId);
    int hod = 16;
    enviarDatos(clientfd, "hod", hod);
    double mean_time = solicitarBusqueda(clientfd);
    printf("Respuesta: %d,%d,%d:%f\n", sourceId, dstId, hod, mean_time);
    cerrarServidor(clientfd);
    close(clientfd);
    // 1397.7
}