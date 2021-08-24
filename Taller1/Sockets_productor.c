#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <time.h>


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


#define PORT 3535
#define BACKLOG 32
#define NumExp 3

struct timespec begin, end; 
int r;

/*
Crear apuntadores a los archivos binarios
*/
FILE* openFile(FILE* file, char* dir, char* modo){
    file = fopen(dir, modo);
    if(file == NULL){
        printf("Error leyendo archivo\n");
    }else{
        //printf("Archivo leido con exito\n");
    }
    return file;
}


/*
Verificar errores
*/
int verificar(int desc, char* message){
    if(desc < 0){
        perror(message);
        exit(-1);
    }
    return desc;
}


int main(int argc, char *argv[]){ 

    // Conseguir la ruta del archivo
    char *ruta = malloc(11*sizeof(char));
    strcat(strcpy(ruta, "./Archivos/"), argv[1]);

    // Conseguir tamaño del archivo
    int tamano = strtol(argv[2], NULL, 10);
    if(strcmp(argv[3], "Mb") == 0){
        tamano = tamano*1000000;
    }else{
        tamano = tamano*1000;
    }

    int serverfd, clientfd, opt,s = 1;
    struct sockaddr_in server, client;
    socklen_t tamanoSock = sizeof(struct sockaddr_in);

    verificar(serverfd = socket(AF_INET, SOCK_STREAM, 0), "\n-->Error en socket():");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 

    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    verificar(s = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr)), 
          "\n-->Error en bind(): ");
    
    verificar(s = listen(serverfd, BACKLOG), "\n-->Error en Listen(): ");
    
    verificar(clientfd = accept(serverfd, (struct sockaddr *)&client, &tamanoSock), 
                "\n-->Error en accept: ");

    FILE *file;
    file = openFile(file, ruta, "r+");

    char *buffer = malloc(tamano*sizeof(char)); 
    fread(buffer, tamano, sizeof(char), file);    

    // Hacer NumExp experimentos
    double tiempoSum = 0;
    for(int i = 0; i < NumExp; i++){

        // Capturar el tiempo de inicio
        clock_gettime(CLOCK_REALTIME, &begin);

        // Enviar datos al cliente
        verificar(r = send(clientfd, buffer, tamano, 0), 
                "\n-->Error en send(): ");

        // Recibir mensaje de confirmación
        char *respuesta = malloc(30*sizeof(char));
        r = recv(clientfd, respuesta, 30, 0); 
        //printf("%s\n", respuesta);
        free(respuesta);

        // Capturar el tiempo de fin
        clock_gettime(CLOCK_REALTIME, &end);

        // Calcular el tiempo transcurrido
        long seconds = end.tv_sec - begin.tv_sec;
        long nanoseconds = end.tv_nsec - begin.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;

        tiempoSum = tiempoSum + elapsed;
    }

    printf("Tiempo medio de énvio usando sockets(): %f segundos\n", tiempoSum/NumExp);

    fclose(file);
    free(buffer);
    close(clientfd);

    free(ruta);

    return 0;
}
