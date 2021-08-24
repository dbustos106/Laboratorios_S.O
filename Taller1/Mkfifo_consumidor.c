#include <sys/stat.h>   // crear/utilizar mkfifo
#include <unistd.h>     // ficheros
#include <fcntl.h>      // ficheros
#include <stdio.h>      // printer
#include <stdlib.h>
#include <string.h>

#define NumExp 5

int main(int argc, char *argv[]){

    // Conseguir tamaño del archivo
    int tamano = strtol(argv[2], NULL, 10);
    if(strcmp(argv[3], "Mb") == 0){
        tamano = tamano*1000000;
    }else{
        tamano = tamano*1000000;
    }


    char* buf = (char*) malloc(tamano*sizeof(char));

    // Hacer NumExp experimentos
    for(int i = 0; i < NumExp; i++){

        // Abrir la primera tuberia
        int fd, r;
        fd = open("/tmp/mkfifo", O_RDONLY);

        // Crear la segunda tuberia
        int fd2;
        mkfifo("/tmp/mkfifo2", 0666);
        fd2 = open("/tmp/mkfifo2", O_WRONLY);

        // Leer datos de la tuberia
        int tam = 0;
        while((r = read(fd, buf + tam, tamano*sizeof(char)-tam)) > 0){
            tam = tam + r;
            if(tam >= tamano){
                break;
            }
        }
        *(buf + tam) = 0;
        fflush(stdout); 
    
        //printf("Mensaje: %s\n", buf);
        close(fd);        

        // Enviar mensaje de confirmación
        write(fd2, "Mensaje recibido", 16);
        close(fd2);
    }

    free(buf);

    return 0;   
}
