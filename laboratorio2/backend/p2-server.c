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

pthread_mutex_t mutex;
ClientEstruct* clientes;

char* strasign(char* chr, int tam, char* buffer, int num, char delimiter);
int verificar(int desc, char* message);
void* handle_conection(void* pclient);

int main(){
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client;
    socklen_t tamano = sizeof(struct sockaddr_in);

    // Se crean las variables de control para la sección crítica
    pthread_mutex_init(&mutex, NULL);

    verificar(serverfd = socket(AF_INET, SOCK_STREAM, 0), "\n-->Error en socket():");
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 
    
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    verificar(r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr)), 
          "\n-->Error en bind(): ");
    
    verificar(r = listen(serverfd, BACKLOG), "\n-->Error en Listen(): ");
    
    clientes = malloc(32*sizeof(ClientEstruct));

    while(true){
        verificar(clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano), 
                "\n-->Error en accept: ");

        char* dirIp = inet_ntoa(client.sin_addr);
        fflush(stdout);

        ClientEstruct* clientEstruct = malloc(sizeof(ClientEstruct));
        clientEstruct->clientfd = clientfd;
        clientEstruct->clientIP = dirIp;

        //while(//dormir hasta que alguna de las pos se libere){
         //   sleep(0.5);
        //}

        pthread_t t;
        pthread_create(&t, NULL,(void *) handle_conection, clientEstruct);     
    }
    return 0;
}

void* handle_conection(void *pclient){
    ClientEstruct clientEstruct = *((ClientEstruct *) pclient);
    int clientfd = clientEstruct.clientfd;
    int sourceId = 0, dstId = 0, hod = 0;
    char* dirIp = "][Cliente:";
    FILE *log;

    free(pclient);

    while(true){
        int r, tam = 0;
        char* buffer = (char*) malloc(60*sizeof(char));

        while((r = recv(clientfd, buffer, 60*sizeof(char)-tam, 0)) > 0){
            tam = tam + r;
            if(tam >= 60-1 || *(buffer + tam-1)  == '\n'){
                break;
            } 
        }

        verificar(r, "\n-->Error en recv(): ");
        *(buffer+tam-1) = 0; 
        printf("Request: %s\n", buffer);
        fflush(stdout);

        // Interpretar el mensaje recibido
        char* metodo = (char*) malloc(5*sizeof(char));
        metodo = strasign(metodo, 5, buffer, 7, ',');

        if(strcmp(metodo, "PUT") == 0){

            char* numero = (char*) malloc(8*sizeof(char));
            char* campo = (char*) malloc(8*sizeof(char));
            campo = strasign(campo, 8, buffer, 12, ':');
    
            if(strcmp(campo, "sourceid") == 0){
                numero = strasign(numero, 8, buffer, 21, '\n');
                sourceId = atoi(numero);
            }else if(strcmp(campo, "dstid") == 0){
                numero = strasign(numero, 8, buffer, 18, '\n');
                dstId = atoi(numero);
            }else{
                numero = strasign(numero, 8, buffer, 16, '\n');
                hod = atoi(numero);
            }

            // Envio respuesta al cliente
            int respuesta = 200;
            verificar(r = send(clientfd, &respuesta, 4, 0), "Error en el send");

            free(campo);
            free(numero);

        }else if(strcmp(metodo, "GET") == 0){
            double mean_time = busqueda(sourceId, dstId, hod);
            verificar(r = send(clientfd, &mean_time, 8, 0), "Error en el send");
            
        }else{
            break;
        }

        printf("sourceid: [%d]\n", sourceId);
        printf("dstid: [%d]\n", dstId);
        printf("hod: [%d]\n", hod);

        // Escritura del archivo log
        struct timespec currentTime; 
        clock_gettime(CLOCK_REALTIME, &currentTime);

        pthread_mutex_lock(&mutex);

        log = openFile(log, "./Archivos/log.txt", "a");
        // ESCRITURAAAAA
        //dd
        fclose(log);

        pthread_mutex_unlock(&mutex); 

        free(buffer);
        free(metodo);
    }

    close(clientEstruct.clientfd);
    printf("Closing connection\n");

    return NULL;
}

char* strasign(char* chr, int tam, char* buffer, int num, char delimiter){
    for(int i = 0; i < tam; i++){
        *(chr + i) = *(buffer + num + i);
        if(*(buffer+num+i+1) == delimiter){
            *(chr+i+1) = 0;
            break; 
        }
    }
    return chr;
} 

int verificar(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}
