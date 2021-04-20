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

    shmId = shmget(key, sizeof(double), 0666 | IPC_CREAT);
    if(shmId < 0){
        printf("Error en shmget");
        exit(-1);
    }
    ap = (double*)shmat(shmId, 0, 0);
    if(ap < 0){
        perror("Error en shamt");
        exit(-1);
    }


    while(*ap == 0 && *(ap + 1) == 0 && *(ap +2) == 0){
        usleep(500000);
    }
    *(ap + 3) = busqueda((int)*(ap),(int)*(ap+1),(int)*(ap+2));



    r = shmdt(ap);
    if(r < 0){
        perror("Error en shmdt");
        exit(-1);
    }

    shmctl(shmId, IPC_RMID, 0);
    return 0;
}