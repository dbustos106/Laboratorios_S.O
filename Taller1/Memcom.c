#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define NumExp 5

struct timespec begin, end; 
int pipefd1[2], pipefd2[2];
key_t key = 1234;
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


void productor(char *ruta, int tamano){

    char *ap;
    int shmId;

    // Crear un espacio de memoria compartida con key = 1234
    shmId = shmget(key, tamano, 0666 | IPC_CREAT);
    if(shmId < 0){
        printf("Error en shmget creacion \n");
        exit(-1);
    }

    // Crear un apuntador al espacio de memoria compartido
    ap = (char*)shmat(shmId, 0, 0);
    if(ap < 0){
        perror("Error en shamt creacion");
        exit(-1);
    }


    FILE *file;
    file = openFile(file, ruta, "r");

    char *buffer = malloc(tamano*sizeof(char));
    fread(buffer, tamano, sizeof(char), file);


    // Hacer NumExp experimentos
    double tiempoSum = 0;
    for(int i = 0; i < NumExp; i++){

        // Capturar el tiempo de inicio
        clock_gettime(CLOCK_REALTIME, &begin);


        // Escribir datos en la memoria compartida
        strncpy(ap, buffer, tamano);   

        // Habilitamos la lectura por parte del consumidor
        r = write(pipefd1[1], "c", sizeof(char));
        

        // Esperamos mensaje de confirmacion
        char aux;
        r = read(pipefd2[0], &aux, sizeof(char));
        
        // Recibir mensaje de confirmación
        char *respuesta = malloc(16*sizeof(char));
        strncpy(respuesta, ap, 16);  
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
    
    printf("Tiempo medio de envío usando memCom(): %f segundos\n", tiempoSum/NumExp);

    free(buffer);
    fclose(file);
    close(pipefd1[1]);
    close(pipefd2[0]);

    // Desvincular el apuntador ap al espacio de memoria compartida
    r = shmdt(ap);
    if(r < 0){
        perror("Error en shmdt");
        exit(-1);
    }

    // Eliminamos el espacio de memoria compartida
    shmctl(shmId, IPC_RMID, 0);

    exit(0);
}


void consumidor(int tamano){

    char *ap;
    int shmId;

    // Recuperar la Id del espacio memoria compartido con key = 1234
    shmId = shmget(key, sizeof(double), 0666);
    if(shmId < 0){
        printf("Error en shmget");
        exit(-1);
    }

    // Crear un apuntador al espacio de memoria compartido
    ap = (char*)shmat(shmId, 0, 0);
    if(ap < 0){
        perror("Error en shamt");
        exit(-1);
    }
    
    char* buffer = (char*) malloc(tamano*sizeof(char));

    // Hacer NumExp experimentos
    for(int i = 0; i < NumExp; i++){

        // Leer datos de la tuberia
        char aux;
        r = read(pipefd1[0], &aux, sizeof(char));        

        // Leer datos de la memoria compartida
        strncpy(buffer, ap, tamano);
        //printf("buffer: %s \n", buffer);
    
        // Escribir mensaje de confirmación
        strncpy(ap, "mensaje recibido", 16);
        
        // Habilitar la lectura del mensaje de confirmación
        r = write(pipefd2[1], "d", 1);        

    }

    free(buffer);
    close(pipefd1[0]);
    close(pipefd2[1]);

    // Desvincular el apuntador ap al espacio de memoria compartida
    r = shmdt(ap);
    if(r < 0){
        perror("Error en shmdt");
        exit(-1);
    }

    // Eliminamos el espacio de memoria compartida
    shmctl(shmId, IPC_RMID, 0);

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
