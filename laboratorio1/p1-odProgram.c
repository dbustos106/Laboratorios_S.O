#include <stdio.h>
#include <stdlib.h>

int main(){

    int opcion;

    do{
        printf("1) Ingresar origen \n");
        printf("2) Ingresar destino \n");
        printf("3) Ingresar hora \n");
        printf("4) Buscar tiempo de viaje medio \n");
        printf("5) Salir \n");
        printf("Ingrese la opción: ");
        scanf("%d", &opcion);


        switch(opcion){
            case 1:
                printf("Ingrese el ID del origen: ");
                int sourceid;
                scanf("%d", &sourceid);
            break;

            case 2:
                printf("Ingrese el ID del destino: ");
                int dstid;
                scanf("%d", &dstid);
            break;

            case 3:
                printf("Ingrese la hora del día: ");
                int hod;
                scanf("%d", &hod);
            break;

            case 4:
                printf("Búsqueda de tiempo de viaje medio iniciada \n");

            break;

            case 5: break;

            default:
                printf("Opción incorrecta, seleccione una opción válida \n");

        }
    }while(opcion != 5);
}