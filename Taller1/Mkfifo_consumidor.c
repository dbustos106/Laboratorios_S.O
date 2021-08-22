#include <sys/stat.h>   // crear/utilizar mkfifo
#include <unistd.h>     // ficheros
#include <fcntl.h>      // ficheros
#include <stdio.h>      // printer
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

    // Conseguir tamaño del archivo
    int tamano = strtol(argv[2], NULL, 10);
    if(strcmp(argv[3], "Mb") == 0){
        tamano = tamano*1000000;
    }else{
        tamano = tamano*1000000;
    }

    int fd, r;
    fd = open("/tmp/mkfifo", O_RDONLY);

    char* buf = (char*) malloc(tamano*sizeof(char));

    int tam = 0;
    // Leer datos de la tuberia
    while((r = read(fd, buf + tam, tamano*sizeof(char)-tam)) > 0){
        tam = tam + r;
        if(tam >= tamano){
            break;
        }
    }
    *(buf + tam) = 0;
    fflush(stdout); 
    
    free(buf);
    close(fd);

    int fd2;
    // Enviar mensaje de confirmación
    mkfifo("/tmp/mkfifo2", 0666);
    fd2 = open("/tmp/mkfifo2", O_WRONLY);
    write(fd2, "Mensaje recibido", 17);
    
    close(fd2);
    
    return 0;   
}