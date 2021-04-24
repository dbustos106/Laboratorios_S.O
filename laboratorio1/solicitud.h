#ifndef solicitud_h
#define solicitud_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

double hacerSolicitud(double sourceId, double dstId, double hod){
    pid_t pid;
    key_t key = 1234;
    int shmId;
    double a,b,c, *ap;
    int status, cpid, r;

    //Recuperar la Id del espacio memoria compartido con key = 1234
    shmId = shmget(key, sizeof(double), 0666);
    if(shmId < 0){
        printf("Error en shmget");
        exit(-1);
    }

    //Crear un apuntador al espacio de memoria compartido
    ap = (double*)shmat(shmId, 0, 0);
    if(ap < 0){
        perror("Error en shamt");
        exit(-1);
    }


    //Almacenar los valores de origen, destino y hora
    *ap = sourceId;
    *(ap + 1) = dstId;
    *(ap + 2) = hod;

    //Dormir el proceso mientras el servidor reponde
    while(*(ap + 3) == 0){
        usleep(50000);
    }
    double mean_time = *(ap + 3);

    //Reiniciar valores
    *(ap + 3) = 0;

    //Desvincular el apuntador ap al espacio de memoria compartida
    r = shmdt(ap);
    if(r < 0){
        perror("Error en shmdt");
        exit(-1);
    }

    return mean_time;
}

void cerrarServer(){
    pid_t pid;
    key_t key = 1234;
    int shmId;
    double a,b,c, *ap;
    int status, cpid, r;

    //Recuperar la Id del espacio memoria compartido con key = 1234
    shmId = shmget(key, sizeof(double), 0666);
    if(shmId < 0){
        printf("Error en shmget");
        exit(-1);
    }

    //Crear un apuntador al espacio de memoria compartido
    ap = (double*)shmat(shmId, 0, 0);
    if(ap < 0){
        perror("Error en shamt");
        exit(-1);
    }

    *(ap + 4) = -1;
    
    //Desvincular el apuntador ap al espacio de memoria compartida
    r = shmdt(ap);
    if(r < 0){
        perror("Error en shmdt");
        exit(-1);
    }

    //Eliminamos el espacio de memoria compartida
    shmctl(shmId, IPC_RMID, 0);
}

#endif