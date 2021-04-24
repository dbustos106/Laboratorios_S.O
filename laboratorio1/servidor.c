#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "busqueda.h"

int main(){
    pid_t pid;
    key_t key = 1234;
    int shmId;
    double a,b,c, *ap;
    int status, cpid, r;

    //Crear un espacio de memoria compartida con key = 1234
    shmId = shmget(key, sizeof(double), 0666 | IPC_CREAT);
    if(shmId < 0){
        printf("Error en shmget");
        exit(-1);
    }

    ////Crear un apuntador al espacio de memoria compartido
    ap = (double*)shmat(shmId, 0, 0);
    if(ap < 0){
        perror("Error en shamt");
        exit(-1);
    }
 
    while(*(ap + 4) != -1){
        //Dormir el proceso mientras el cliente hace la solicitud
        while(*ap == 0 && *(ap + 1) == 0 && *(ap + 2) == 0){
            usleep(50000);
        }
        //Realizar la busqueda del dato
        *(ap + 3) = busqueda((int)*(ap),(int)*(ap + 1),(int)*(ap + 2));
        //Reiniciar los valores
        *ap = 0;
        *(ap + 1) = 0;
        *(ap + 2) = 0;
    }

    //Desvincular el apuntador ap al espacio de memoria compartida
    r = shmdt(ap);
    if(r < 0){
        perror("Error en shmdt");
        exit(-1);
    }

    //Eliminamos el espacio de memoria compartida
    shmctl(shmId, IPC_RMID, 0);

    return 0;
}