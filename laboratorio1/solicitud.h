#ifndef HashTable_h
#define HashTable_h

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

void hacerSolicitud(){
    pid_t pid;
    key_t key = 1234;
    int shmId;
    float a,b,c, *ap;
    int status, cpid, r;

    shmId = shmget(key, sizeof(double), 0666| IPC_CREAT);
    if(shmId < 0){
        printf("Error en shmget");
        exit(-1);
    }
    ap = (float*) shmat(shmId, 0, 0);
    if(ap < 0){
        
    }

}



#endif