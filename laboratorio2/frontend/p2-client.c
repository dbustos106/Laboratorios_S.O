#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "solicitud.h"

void validarRespuesta(int respuesta);

int main(int argc, char *argv[]){ 

    int opcion = -1;
    int sourceid = -1;
    int dstid = -1;
    int hod = -1;

    struct timespec begin, end; 
    int clientfd = hacerConexion(argv[1]);
    int respuesta;

    do{
        printf("---- MENU DEL CLIENTE ----\n");
        printf("1) Ingresar origen \n");
        printf("2) Ingresar destino \n");
        printf("3) Ingresar hora \n");
        printf("4) Buscar tiempo de viaje medio \n");
        printf("5) Salir \n\n");
        printf("Ingrese la opción: ");
        scanf("%d", &opcion);
        
        switch(opcion){
            case 1:
                printf("Ingrese el ID del origen: ");
                scanf("%d", &sourceid);
                respuesta = enviarDatos(clientfd, "sourceid", sourceid);
                validarRespuesta(respuesta);
            break;

            case 2:
                printf("Ingrese el ID del destino: ");
                scanf("%d", &dstid);
                respuesta = enviarDatos(clientfd, "dstid", dstid);
                validarRespuesta(respuesta);
            break;

            case 3:
                printf("Ingrese la hora del día: ");
                scanf("%d", &hod);
                respuesta = enviarDatos(clientfd, "hod", hod);
                validarRespuesta(respuesta);
            break;

            case 4:
                printf("Búsqueda de tiempo de viaje medio iniciada \n");

                // Capturar el tiempo de inicio
                clock_gettime(CLOCK_REALTIME, &begin);
                
                double mean_time = solicitarBusqueda(clientfd);

                if(mean_time == -1){
                    printf("NA\n");
                }else{
                    printf("Tiempo medio: %f\n", mean_time);
                }
                // Capturar el tiempo de fin
                clock_gettime(CLOCK_REALTIME, &end);

                long seconds = end.tv_sec - begin.tv_sec;
                long nanoseconds = end.tv_nsec - begin.tv_nsec;
                double elapsed = seconds + nanoseconds*1e-9;
                printf("Tiempo: %f segundos\n", elapsed);
            break;

            case 5: 
                //cerrarServidor(clientfd);
                close(clientfd);
            break;

            default:
                printf("Opción incorrecta, seleccione una opción válida \n");
            break;

        }
    }while(opcion != 5);

    return 0;
}

void validarRespuesta(int respuesta){
    int cg;
    if(respuesta == 200){
        printf("\nSe registró el valor con exito, ingrese cualquier tecla para continuar: ");
        scanf("%d", &cg);
        printf("\n");
    }else{
        printf("\nError, el servidor no ha registrado el valor del número\n");
    }
    return;
}