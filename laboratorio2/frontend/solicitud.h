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

int check(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}

int enviarDatos(int clientfd, char *campo,int num){
    int r;
    char *datos = malloc(60*sizeof(char));
    char respuesta[32];
    char str[10];

    sprintf(str, "%d", num);
    strcat(datos, "{metodo:PUT, payload:{");
    strcat(datos, campo);
    strcat(datos, ":");
    strcat(datos, str);
    strcat(datos, "}}");

    check(r = send(clientfd, datos, 60, 0), "\n-->Error en send(): ");
    free(datos);

    // Se recibe la respuesta del servidor
    r = recv(clientfd, respuesta, 32, 0);
    respuesta[r] = 0;
    printf("Respuesta: %s\n", respuesta);

    return strtol(respuesta, NULL, 10);
}

double solicitarBusqueda(int clientfd){
    printf("sold\n");
    int r;
    double mean_time = 0;
    char respuesta[32];

    printf("llego aqui");
    check(r = send(clientfd, "{metodo:GET, data:mean_time}", 60, 0), "\n-->Error en send(): ");

    // Se recibe la respuesta del servidor
    r = recv(clientfd, respuesta, 32, 0);
    respuesta[r] = 0;
    printf("Respuesta: %s\n", respuesta);

    return mean_time;
}

int hacerConexion(char* dirIp){
    int clientfd, r;
    struct sockaddr_in client;
    struct hostent *he;
    char buffer[32];
    
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }

    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);

    inet_aton(dirIp, &client.sin_addr);
    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en connect(): ");
        exit(-1);
    }
    return clientfd;
}

#endif