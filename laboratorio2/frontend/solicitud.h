#ifndef solicitud_h
#define solicitud_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
 
#define PORT 3535

int verificar(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}

int enviarDatos(int clientfd, char *campo, int num){
    int r, respuesta;
    char str[10];
    sprintf(str, "%d", num);

    // Se concatena el mensaje
    char *datos = malloc(60*sizeof(char));
    strcat(datos, "metodo:PUT, ");
    strcat(datos, campo);
    strcat(datos, ":");
    strcat(datos, str);

    // Se envia mensaje al servidor
    verificar(r = send(clientfd, datos, 60, 0), 
        "\n-->Error en send(): ");

    printf("Se envió el mensaje y se espera respuesta\n");

    // Se recibe la respuesta del servidor
    r = recv(clientfd, &respuesta, 4, 0);
    printf("Respuesta: %d ", respuesta);
    free(datos);
    return respuesta;
}

double solicitarBusqueda(int clientfd){
    int r;
    double mean_time;

    // Se envia mensaje al servidor
    verificar(r = send(clientfd, "metodo:GET, data:mean_time", 60, 0), 
        "\n-->Error en send(): ");

    // Se recibe la respuesta del servidor
    r = recv(clientfd, &mean_time, 8, 0);
    printf("Respuesta: %f\n", mean_time);

    return mean_time;
}

void cerrarServidor(int clientfd){
    int r;
    double mean_time;

    // Se envia mensaje al servidor
    verificar(r = send(clientfd, "metodo:close", 60, 0), 
        "\n-->Error en send(): ");

}

int hacerConexion(char* dirIp){
    int clientfd, r;
    struct sockaddr_in client;
    struct hostent *he;
    char buffer[32];
    
    verificar(clientfd = socket(AF_INET, SOCK_STREAM, 0), 
        "\n-->Error en socket():");

    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);

    inet_aton(dirIp, &client.sin_addr);

    verificar(r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr)), 
        "\n-->Error en connect(): ");

    return clientfd;
}

#endif