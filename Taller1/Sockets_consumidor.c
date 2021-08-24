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
#define NumExp 3

int r;

int verificar(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}


int hacerConexion(char* dirIp){
    int clientfd, r;
    struct sockaddr_in client;
    
    verificar(clientfd = socket(AF_INET, SOCK_STREAM, 0), 
        "\n-->Error en socket():");

    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);

    inet_aton(dirIp, &client.sin_addr);

    verificar(r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr)), 
        "\n-->Error en connect(): ");

    return clientfd;
}


int main(int argc, char *argv[]){  
    
    // Conseguir tamaño del archivo
    int tamano = strtol(argv[2], NULL, 10);
    if(strcmp(argv[3], "Mb") == 0){
        tamano = tamano*1000000;
    }else{
        tamano = tamano*1000;
    }

    int clientfd = hacerConexion("127.0.0.1");
    
    char* buffer = (char*) malloc(tamano*sizeof(char));

    // Hacer NumExp experimentos
    for(int i = 0; i < NumExp; i++){

        // Recibir el mensaje
        int tam = 0;
        while((r = recv(clientfd, buffer + tam, tamano*sizeof(char)-tam, 0))  > 0){
            tam = tam + r;
            if(tam >= tamano){
                break;
            }
        }
        *(buffer + tam) = 0;
        fflush(stdout);

        //printf("Mensaje: %s\n", buffer);

        // Enviar mensaje de confirmación
        verificar(r = send(clientfd, "Confirmacion de mensaje\n", 30, 0), 
                "\n-->Error en send(): ");
    }

    free(buffer);

    close(clientfd);
}