#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//nombre
#define SMOBJ_NAME2 "myMemoryObj2"

int main(void){
    //crear el filedescriptor
    int fd;
    // creacion en modo lectura escritura
    fd = shm_open(SMOBJ_NAME2, O_CREAT | O_RDWR, 00600);
    if(fd == -1){
        printf("Error en la creacion de la memoria compartida");
    }
    // declaracion del tamño de la memoria compartida
    if (ftruncate(fd, 20000) == -1){
        printf("Error en la declaracion del tamaño de la memoria compartida");

    }
}