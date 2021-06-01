#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include "busqueda.h"
#include "./Estructuras/Queue.h"
 
#define PORT 3535
#define BACKLOG 32
#define THREAD_SIZE 32

pthread_t hilos[THREAD_SIZE];
pthread_mutex_t mutex;

FILE* openFile(FILE* file, char* dir, char* modo);
int verificar(int desc, char* message);
void* handle_conection(void* pclient);
void* function(void* arg);

int main(){
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client;
    socklen_t tamano = sizeof(struct sockaddr_in);

    // Se crean las variables de control para la sección crítica
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < THREAD_SIZE; i++){
        pthread_create(&hilos[i], NULL,(void *) function, NULL);
    }

    verificar(serverfd = socket(AF_INET, SOCK_STREAM, 0), "\n-->Error en socket():");
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 
    
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    verificar(r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr)), 
          "\n-->Error en bind(): ");
    
    verificar(r = listen(serverfd, BACKLOG), "\n-->Error en Listen(): ");
    
    while(true){
        verificar(clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano), 
                "\n-->Error en accept: ");

        char* dirIp = inet_ntoa(client.sin_addr);
        fflush(stdout);

        ClientEstruct* clientEstruct = malloc(sizeof(ClientEstruct));
        clientEstruct->clientfd = clientfd;
        clientEstruct->clientIP = dirIp;
    
        pthread_mutex_lock(&mutex);
        enqueue(clientEstruct);
        pthread_mutex_unlock(&mutex);       
    }
    return 0;
}

void* function(void * arg){
    while(true){
        pthread_mutex_lock(&mutex);
        ClientEstruct* clientfd = dequeue();
        pthread_mutex_unlock(&mutex);
        if(clientfd != NULL){
            handle_conection(clientfd);
        }
    }
}

int verificar(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}

void* handle_conection(void *pclient){
    ClientEstruct clientEstruct = *((ClientEstruct *) pclient);
    int clientfd = clientEstruct.clientfd;
    int sourceId = 0, dstId = 0, hod = 0;
    char* dirIp = "][Cliente:";
    FILE *log;

    //strcat(dirIp, *clientEstruct.clientIP);
    free(pclient);

    while(true){
        int r, tam = 0;
        char buffer[60];

        while((r = recv(clientfd, buffer, sizeof(buffer)-tam-1, 0)) > 0){
            tam = tam + r;
            if(tam >= 60-1 || buffer[tam-1] == '\n'){
                break;
            } 
        }
        verificar(r, "\n-->Error en recv(): ");
        buffer[tam-1] = 0;
        
        printf("Request: %s\n", buffer);
        fflush(stdout);

        // Interpretar el mensaje recibido
        char* campo;
        char* numero;
        char* metodo = "PUT";
        char* metodo2 = "GET";
        char* metodo3 = "CLOSE";

        if((numero = strstr(buffer, metodo)) != NULL) {
            if(strstr(buffer, "sourceid") != NULL){
                campo = "][sourceid:";
                numero = numero + 14;
                sourceId = atoi(numero);
            }else{
                if(strstr(buffer, "dstid") != NULL){
                    campo = "][dstid:";
                    numero = numero + 11;
                    dstId = atoi(numero);
                }else{
                    campo = "][hod:";
                    numero = numero + 9;
                    hod = atoi(numero);
                }
            }

            int respuesta = 200;
            verificar(r = send(clientfd, &respuesta, 4, 0), "Error en el send");

        }else if(strstr(buffer, metodo2) != NULL){
            campo = "][busqueda:";
            double mean_time = busqueda(sourceId, dstId, hod);
            verificar(r = send(clientfd, &mean_time, 8, 0), "Error en el send");
            //sprintf(numero, "%f", mean_time);

        }
        printf("Num: %s\n", campo);
        printf("Num: %s\n", numero);
        // Escritura del archivo log
        //struct timespec currentTime; 
        //clock_gettime(CLOCK_REALTIME, &currentTime);

        //log = openFile(log, "./Archivos/log.txt", "a");
        //pthread_mutex_lock(&mutex);
        //fprintf(log, "%s", clientEstruct.clientIP);
        //fprintf(log, "%s", dirIp);
        //pthread_mutex_unlock(&mutex); 

        //free(numero);
        //fclose(log);
        //free(campo);
    }


    close(clientfd);
    printf("Closing connection\n");

    return NULL;
}