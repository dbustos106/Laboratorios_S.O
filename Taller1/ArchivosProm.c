#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NumExp 1
#define FileName "data.dat"
int pipefd[2];

struct timespec begin, end; 
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


void productor(FILE* data, char *ruta, int tamano){

    FILE *file;
    file = openFile(file, ruta, "r+");

    char *buf = malloc(tamano*sizeof(char));
    fread(buf, tamano, sizeof(char), file);
    
    // Hacer NumExp experimentos
    double tiempoSum = 0;
    for(int i = 0; i < NumExp; i++){

        // Capturar el tiempo de inicio
        clock_gettime(CLOCK_REALTIME, &begin);

        //// Escribir datos en el fichero
        //fwrite(buf,sizeof(char), tamano, data);

        // Habilitar la lectura del consumidor
        r = write(pipefd[1], "d", sizeof(char));
        printf("Ya habilitamos la lectura\n");

        // Esperar mensaje de confirmacion
        char aux;
        r = read(pipefd[0], &aux, 1);
        printf("AuxProd %c\n", aux);

        //// Recibir mensaje de confirmación
        //char* respuesta = malloc(24*sizeof(char));
        //// ....
        ////printf("Mensaje de confirmacion: %s\n", respuesta);
        //free(respuesta);

        // Capturar el tiempo de fin
        clock_gettime(CLOCK_REALTIME, &end);

        // Calcular el tiempo transcurrido
        long seconds = end.tv_sec - begin.tv_sec;
        long nanoseconds = end.tv_nsec - begin.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;

        tiempoSum = tiempoSum + elapsed;
    }
    
    printf("Tiempo medio de envío usando ficheros(): %f segundos\n", tiempoSum/NumExp);

    free(buf);
    fclose(file);

}


void consumidor(int tamano){

    // Abrir fichero para lectura
    FILE *data;
    data = openFile(data, "./data.txt", "rb");
    
    char* buffer = (char*) malloc(tamano*sizeof(char));

    // Hacer NumExp experimentos
    for(int i = 0; i < NumExp; i++){

        // Espere a que el productor escriba  

        char aux;  
        r = read(pipefd[0], &aux, sizeof(char));
        printf("AUXsssss: %c\n", aux);
        printf("HOLA MUNDOOO\n");
        //printf("Ogia, ya puede leer\n");
        //// Leer datos del fichero
        //fread(buffer, sizeof(char), tamano, data);
        //
        //printf("buffer: %s \n", buffer);
    
        // Escribir mensaje de confirmacion7
        // ....

        r = write(pipefd[1], "c", 1);
        
    

    }

    fclose(data);
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
    
    FILE *data;
    data = openFile(data, "./data.txt", "w+");  

    r = pipe(pipefd);
    if(r < 0){
        perror("error pipe1()");
        exit(-1);
    }

    pid = fork();
    if(pid < 0){
        perror("error fork");
        exit(-1);
    }

    if(pid == 0){  
        consumidor(tamano);
    }else{
        productor(data, ruta, tamano);
    }

    free(ruta);
    fclose(data);

    return 0;
}
