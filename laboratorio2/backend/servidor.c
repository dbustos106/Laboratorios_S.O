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
 
#define PORT 3536
#define BACKLOG 32

FILE* openFile(FILE* file, char* dir, char* modo);
int check(int desc, char* message);
void * handle_conection(void *pclient);


int main(){
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client;
    socklen_t tamano;

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

        pthread_t t;
        int *pclient = malloc(sizeof(int));
        *pclient = clientfd;
        pthread_create(&t, NULL, (void *) handle_conection, pclient);
        if(r != 0){
            perror("\n-->pthread_create error: ");
            exit(-1);
        }

        struct in_addr clientIP;
        clientIP = client.sin_addr;
        char ipStr[INET_ADDRSTRLEN];
        printf("IP Origen: %s\n",inet_ntop(AF_INET, &clientIP, ipStr, INET_ADDRSTRLEN));
    }

    close(serverfd);
    return 0;
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

void * handle_conection(void *pclient){

    int clientfd = *((int *) pclient);
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

    int respuesta = 2;
    check(r = send(clientfd, &respuesta, 4, 0), "Error en el send");

    FILE *log;
    log = openFile(log, "./Archivos/log.txt", "w+");

    close(clientfd);
    printf("Closing connection\n");

    return NULL;
}
