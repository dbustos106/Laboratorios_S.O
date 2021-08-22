#include <sys/stat.h>   // crear/utilizar mkfifo
#include <unistd.h>     // ficheros
#include <fcntl.h>      // ficheros
#include <stdio.h>      // printer
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
    
    char*buf = malloc(tamano*sizeof(char));
    fread(buf, tamano, sizeof(char), file);


    int fd,w; 
    mkfifo("/tmp/mkfifo", 0666);
    fd = open("/tmp/mkfifo", O_WRONLY);
    
    int tam = 0;
    // Escribir datos en la tuberia
    while((w = write(fd, buf + tam, tamano*sizeof(char)-tam)) > 0){
        tam = tam + w;
        if(tam >= tamano){
            break;
        }
    }
     
    free(buf);
    close(fd);
    fclose(file);

    int fd2;
    // Recibir mensaje de confirmación
    char *respuesta = malloc(17*sizeof(char));
    fd2 = open("/tmp/mkfifo2", O_RDONLY);
    read(fd2, respuesta, 17);
    //printf("%s\n", respuesta);

    free(respuesta);
    free(ruta);
    close(fd2);

    return 0;
}