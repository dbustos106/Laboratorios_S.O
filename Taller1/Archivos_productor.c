#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FileName "data.dat"


int fd; // Descriptor de archivo
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


void reportar_y_salir(const char* msg) {
    perror(msg);
    exit(-1); 
}


int main(int argc, char *argv[]) {

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


    struct flock lock;
    lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
    lock.l_whence = SEEK_SET; /* base for seek offsets */
    lock.l_start = 0;         /* 1st byte in file */
    lock.l_len = 0;           /* 0 here means 'until EOF' */
    lock.l_pid = getpid();    /* process id */

    if ((fd = open(FileName, O_RDWR | O_CREAT, 0666)) < 0){  /* -1 signals an error */
        report_and_exit("Fallo al abrir el archivo");
    }
    
    if (fcntl(fd, F_SETLK, &lock) < 0) /** F_SETLK doesn't block, F_SETLKW does **/
        report_and_exit("fcntl falló al poner el bloqueo");
    else{

        char *buffer = malloc(tamano*sizeof(char)); 
        fread(buffer, tamano, sizeof(char), file);

        int tam = 0;
        while((r = write(fd, buffer + tam, tamano*sizeof(char)-tam)) > 0){
            tam = tam + r;
            if(tam >= tamano-1){
                break;
            }
        }

        //fprintf(stderr, "El proceso %d ha escrito al archivo.\n", lock.l_pid);
    }

    // Se desbloquea el archivo manualmente. 
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0){
        report_and_exit("Fallo en el desbloqueo manual.");
    }

    // Se cierran los archivos, y por default se debloquean 
    close(fd); 
    fclose(file);
  
    return 0;  
}