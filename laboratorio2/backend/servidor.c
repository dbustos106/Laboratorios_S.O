#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "./Estructuras/Queue.h"
 
#define PORT 3535
#define BACKLOG 32
#define THREAD_SIZE 32

pthread_t hilos[THREAD_SIZE];
pthread_mutex_t mutex;


FILE* openFile(FILE* file, char* dir, char* modo);
int check(int desc, char* message);
void* handle_conection(void* pclient);
void* thread_function(void* arg);

int main(){
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client;
    socklen_t tamano = sizeof(struct sockaddr_in);

    // Se crean las variables de control para la sección crítica
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < THREAD_SIZE; i++){
        pthread_create(&hilos[i], NULL,(void *) thread_function, NULL);
    }

    check(serverfd = socket(AF_INET, SOCK_STREAM, 0), "\n-->Error en socket():");
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 
    
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    check(r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr)), 
          "\n-->Error en bind(): ");
    
    check(r = listen(serverfd, BACKLOG), "\n-->Error en Listen(): ");
    
    while(true){
        printf("Waiting for connections...\n");
        check(clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano), 
                "\n-->Error en accept: ");

        printf("Connected!\n");

        char* dirIp = inet_ntoa(client.sin_addr);
        printf("Se obtuvo una conexion desde %s\n", dirIp);  
        fflush(stdout);

        ClientEstruct* clientEstruct = malloc(sizeof(ClientEstruct));
        clientEstruct->clientfd = clientfd;
        clientEstruct->clientIP = dirIp;
        pthread_mutex_lock(&mutex);
        enqueue(clientEstruct);
        pthread_mutex_unlock(&mutex);
    }

    // Se destruyen las variables de control de la sección crítica
    pthread_mutex_destroy(&mutex);

    close(serverfd);
    return 0;
}

void* thread_function(void * arg){
    while(true){
        pthread_mutex_lock(&mutex);
        ClientEstruct* clientfd = dequeue();
        pthread_mutex_unlock(&mutex);
        if(clientfd != NULL){
            handle_conection(clientfd);
        }
    }
}

FILE* openFile(FILE* file, char* dir, char* modo){
    file=fopen(dir, modo);
    if(file==NULL){
        printf("Error leyendo archivo\n");
    }else{
        printf("Archivo leido con exito\n");
    }
    return file;
}

int check(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}

void* handle_conection(void *pclient){
    ClientEstruct clientf = *((ClientEstruct *) pclient);
    int clientfd = clientf.clientfd;
    free(pclient);
    int r, tam = 0;
    char buffer[60];

    while((r = recv(clientfd, buffer, sizeof(buffer)-tam-1, 0)) > 0){
        tam = tam + r;
        if(tam > 32-1 || buffer[tam-1] == '\n'){
            break;
        } 
    }
    check(r, "\n-->Error en recv(): ");
    buffer[tam -1] = 0;
    
    printf("Request: %s\n", buffer);
    fflush(stdout);

    // Intepretar mensaje

    int respuesta = 2;
    check(r = send(clientfd, &respuesta, 4, 0), "Error en el send");

    FILE *log;
    log = openFile(log, "./Archivos/log.txt", "w+");

    // Seccion critica
        // Escritura de log
    // Fin seccion critica

    close(clientfd);
    printf("Closing connection\n");

    return NULL;
}
