#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


pid_t pid;
int pipefd1[2];
int pipefd2[2];
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

    int tam = 0;
    // Escribir datos en la tuberia
    while((r = write(pipefd1[1], buffer + tam, tamano*sizeof(char)-tam)) > 0){
        tam = tam + r;
        if(tam >= tamano){
            break;
        }
    }

    free(buffer);
    fclose(file);
    close(pipefd1[1]);

    // Recibir mensaje de confirmación
    char *respuesta = malloc(17*sizeof(char));
    r = read(pipefd2[0], respuesta, 16);
    //printf("%s\n", respuesta);

    close(pipefd2[0]);
    free(respuesta);
    
    exit(0);
}


/*
Proceso Consumidor
*/
void consumidor(int tamano){
    char* buffer = (char*) malloc(tamano*sizeof(char));

    int tam = 0;
    // Leer datos de la tuberia
    while((r = read(pipefd1[0], buffer + tam, tamano*sizeof(char)-tam)) > 0){
        tam = tam + r;
        if(tam >= tamano){
            break;
        }
    }
    *(buffer + tam) = 0;
    fflush(stdout); 

    free(buffer);
    close(pipefd1[0]);

    // Enviar mensaje de confirmación
    r = write(pipefd2[1], "Mensaje recibido", 16);
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
        close(pipefd1[0]);
        close(pipefd2[1]);
        productor(ruta, tamano);
    }else{
        close(pipefd1[1]);
        close(pipefd2[0]);
        consumidor(tamano);
    }

    free(ruta);

    return 0;
}
