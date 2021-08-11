#include "./Estructuras/ClientEstruct.h"
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
 
#define PORT 3535
#define BACKLOG 32

pthread_mutex_t mutex;
ClientEstruct* clientes;
int puntero;

char* strasign(char* chr, int tam, char* buffer, int num, char delimiter);
void writeLog(char* dirIp, char* solicitud, char* valor);
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
    puntero = -1;
    
    // Se inicializan las posiciones de los clientes
    for(int i = 0; i < 32; i++){
        (clientes + i)->clientfd = -1;
    }

    while(true){
        verificar(clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano), 
                "\n-->Error en accept: ");

        char* dirIp = inet_ntoa(client.sin_addr);
        fflush(stdout);

        // Ingresar nuevo dato
        do{
            puntero = (puntero + 1) % 32;
        }while((clientes + puntero)->clientfd != -1);

        // Se asignan los datos del nuevo cliente
        (clientes + puntero)->clientfd = clientfd;
        (clientes + puntero)->clientIP = dirIp;

        pthread_t t;
        // Se asigna el cliente a un nuevo hilo
        pthread_create(&t, NULL,(void *) handle_conection, (clientes + puntero));     
    }
    return 0;
}

// Funcion ejecutada por un hilo para atender a un cliente
void* handle_conection(void *pclient){
    ClientEstruct* clientEstruct = (ClientEstruct *) pclient;
    int clientfd = clientEstruct->clientfd;
    char* clientIp = clientEstruct->clientIP;
    int sourceId = 0, dstId = 0, hod = 0;

    while(true){
        int r, tam = 0;
        char* buffer = (char*) malloc(60*sizeof(char));

        // Se recibe el mensaje del cliente
        while((r = recv(clientfd, buffer, 60*sizeof(char)-tam, 0)) > 0){
            tam = tam + r;
            if(tam >= 60-1 || *(buffer + tam-1)  == '\n'){
                break;
            } 
        }

        verificar(r, "\n-->Error en recv(): ");
        
        *(buffer+tam-1) = 0; 
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

            // Bloqueo la sección crítica
            pthread_mutex_lock(&mutex);

            // Se escribe el archivo log
            writeLog(clientIp, campo, numero);

            // Desbloqueo la sección crítica
            pthread_mutex_unlock(&mutex);     

            free(campo);
            free(numero);        

        }else if(strcmp(metodo, "GET") == 0){
            // Se realiza la busqueda
            double mean_time = busqueda(sourceId, dstId, hod);
            verificar(r = send(clientfd, &mean_time, 8, 0), "Error en el send");
            
            char* solicitud = malloc(8*sizeof(char));
            sprintf(solicitud, "%s", "Busqueda");
            char* resultado = malloc(8*sizeof(char));
            sprintf(resultado, "%f", mean_time);

            // Bloqueo la sección crítica
            pthread_mutex_lock(&mutex);

            // Se escribe el archivo log
            writeLog(clientIp, solicitud, resultado);

            // Desbloqueo la sección crítica
            pthread_mutex_unlock(&mutex);  
        }else{
            free(buffer);
            free(metodo);
            break;
        }

        free(buffer);
        free(metodo);
    }

    // Se libera el espacio del cliente
    close(clientEstruct->clientfd);
    clientEstruct->clientfd = -1;

    return NULL;
}

// Función para escribir el archivo log
void writeLog(char* dirIp, char* solicitud, char* valor){
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char* fecha = malloc(128*sizeof(char));
    strftime(fecha,128,"%y/%m/%d %H:%M:%S",tlocal);

    FILE *log;
    log = openFile(log, "./Archivos/log.txt", "a");
    // Escritura del archivo log
    fprintf(log, "%s", "[Fecha:");
    fprintf(log, "%s", fecha);
    fprintf(log, "%s", "] [Cliente:");
    fprintf(log, "%s", dirIp);
    fprintf(log, "%s", "] [");
    fprintf(log, "%s", solicitud);
    fprintf(log, "%s", "-");
    fprintf(log, "%s", valor);
    fprintf(log, "%s", "]\n");

    fclose(log);
}

// Función que completa el puntero chr con la información del buffer
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
