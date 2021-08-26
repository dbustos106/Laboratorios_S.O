#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NumExp 5

struct timespec begin, end; 
int pipefd1[2], pipefd2[2];
pid_t pid;
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
Proceso Productor
*/
void productor(char *ruta, int tamano){

    FILE *file;
    file = openFile(file, ruta, "r+");

    char *buffer = malloc(tamano*sizeof(char)); 
    fread(buffer, tamano, sizeof(char), file);


    // Hacer NumExp experimentos
    double tiempoSum = 0;
    for(int i = 0; i < NumExp; i++){

        // Capturar el tiempo de inicio
        clock_gettime(CLOCK_REALTIME, &begin);

        // Escribir datos en la tuberia
        int tam = 0;
        while((r = write(pipefd1[1], buffer + tam, tamano*sizeof(char)-tam)) > 0){
            tam = tam + r;
            if(tam >= tamano){
                break;
            }
        }

        // Recibir mensaje de confirmación
        char *respuesta = malloc(16*sizeof(char));
        r = read(pipefd2[0], respuesta, 16);
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

    printf("Tiempo medio de énvio usando pipe(): %f segundos\n", tiempoSum/NumExp);

    fclose(file);
    free(buffer);
    close(pipefd1[1]);
    close(pipefd2[0]);
    
    exit(0);
}


/*
Proceso Consumidor
*/
void consumidor(int tamano){
    char* buffer = (char*) malloc(tamano*sizeof(char));

    // Hacer NumExp experimentos
    for(int i = 0; i < NumExp; i++){

        // Leer datos de la tuberia
        int tam = 0;
        while((r = read(pipefd1[0], buffer + tam, tamano*sizeof(char)-tam)) > 0){
            tam = tam + r;
            if(tam >= tamano){
                break;
            }
        }
        *(buffer + tam) = 0;
        fflush(stdout); 

        //printf("Mensaje: %s\n", buffer);

        // Enviar mensaje de confirmación
        r = write(pipefd2[1], "Mensaje recibido", 16);
    }

    free(buffer);
    close(pipefd1[0]);
    close(pipefd2[1]);
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


    r = pipe(pipefd1);
    if(r < 0){
        perror("error pipe1()");
        exit(-1);
    }

    r = pipe(pipefd2);
    if(r < 0){
        perror("error pipe2()");
        exit(-1);
    }


    pid = fork();
    if(pid < 0){
        perror("error fork");
        exit(-1);
    }

    if(pid == 0){  
        close(pipefd1[1]);
        close(pipefd2[0]);
        consumidor(tamano);
    }else{
        close(pipefd1[0]);
        close(pipefd2[1]);
        productor(ruta, tamano);
    }

    free(ruta);

    return 0;
}
