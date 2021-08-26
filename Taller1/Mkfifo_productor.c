#include <sys/stat.h>   // crear/utilizar mkfifo
#include <unistd.h>     // ficheros
#include <fcntl.h>      // ficheros
#include <stdio.h>      // printer
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NumExp 5

struct timespec begin, end; 

// Crear apuntadores a los archivos binarios
FILE* openFile(FILE* file, char* dir, char* modo){
    file = fopen(dir, modo);
    if(file == NULL){
        printf("Error leyendo archivo\n");
    }else{
        //printf("Archivo leido con exito\n");
    }
    return file;
}
    

double main(int argc, char *argv[]){

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
    

    FILE *file;
    file = openFile(file, ruta, "r+");
    
    char *buffer = malloc(tamano*sizeof(char));
    fread(buffer, tamano, sizeof(char), file);


    // Hacer NumExp experimentos
    double tiempoSum = 0;
    for(int i = 0; i < NumExp; i++){

        // Crear la primera tuberia
        int fd,r; 
        mkfifo("/tmp/mkfifo", 0666);
        fd = open("/tmp/mkfifo", O_WRONLY);
    
        // Abrir la segunda tuberia
        int fd2;
        fd2 = open("/tmp/mkfifo2", O_RDONLY);

        // Capturar el tiempo de inicio
        clock_gettime(CLOCK_REALTIME, &begin);

        // Escribir datos en la tuberia
        int tam = 0;
        while((r = write(fd, buffer + tam, tamano*sizeof(char)-tam)) > 0){
            tam = tam + r;
            if(tam >= tamano){
                break;
            }
        }
        close(fd);        

        // Recibir mensaje de confirmación
        char *respuesta = malloc(16*sizeof(char));
        r = read(fd2, respuesta, 16);
        //printf("%s\n", respuesta);

        free(respuesta);
        close(fd2);

        // Capturar el tiempo de fin
        clock_gettime(CLOCK_REALTIME, &end);

        // Calcular el tiempo transcurrido
        long seconds = end.tv_sec - begin.tv_sec;
        long nanoseconds = end.tv_nsec - begin.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;

        tiempoSum = tiempoSum + elapsed;
    }

    printf("Tiempo medio de énvio usando mkfifo(): %f segundos\n", tiempoSum/NumExp);

    free(buffer);
    fclose(file);

    free(ruta);

    return 0;
}
