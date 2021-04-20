#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "solicitud.h"

int main(){    
    int opcion = -1;
    int sourceid = -1;
    int dstid = -1;
    int hod = -1;
    do{
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
            break;

            case 2:
                printf("Ingrese el ID del destino: ");
                scanf("%d", &dstid);
            break;

            case 3:
                printf("Ingrese la hora del día: ");
                scanf("%d", &hod);
            break;

            case 4:
                printf("Búsqueda de tiempo de viaje medio iniciada \n");
	            
                struct timespec begin, end; 
                
                clock_gettime(CLOCK_REALTIME, &begin);
                double mean_time = hacerSolicitud((double) sourceid, (double) dstid, (double) hod);
                
                if(mean_time == -1){
                    printf("NA\n");
                }else{
                    printf("Media: %f\n", mean_time);
                }
                clock_gettime(CLOCK_REALTIME, &end);

                long seconds = end.tv_sec - begin.tv_sec;
                long nanoseconds = end.tv_nsec - begin.tv_nsec;
                double elapsed = seconds + nanoseconds*1e-9;
                printf("Tiempo: %f segundos\n", elapsed);
            break;

            case 5: 
                exit(-1);
            break;

            default:
                printf("Opción incorrecta, seleccione una opción válida \n");

        }
    }while(opcion != 5);
}