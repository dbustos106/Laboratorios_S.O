#ifndef Travel_h
#define Travel_h

#include<stdio.h>
#include<stdlib.h>

typedef struct Travel{
    int sourceid;
    int dstid;
    int hod;
    double mean_travel_time;
    double standard_deviation_travel_time;
    double geometric_mean_travel_time;
    double geometric_standard_deviation_travel_time;
}Travel;

Travel* createTravel(){
    Travel *travel = (Travel*)malloc(sizeof(Travel));
    if(travel == NULL){
        printf("Error en la asignación de memoria del travel\n");
        exit(-1);
    }
    return travel;
}

bool read(Travel* travel, FILE* file){
    bool lectura = true;
    char* comas = (char*) malloc(sizeof(char));

    if(fscanf(file, "%d%c", &travel->sourceid, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%d%c", &travel->dstid, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%d%c", &travel->hod, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->mean_travel_time, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->standard_deviation_travel_time, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->geometric_mean_travel_time, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->geometric_standard_deviation_travel_time, comas) == EOF){
        lectura = false;
    }
    free(comas);
    return lectura;
} 

void writeTravel(FILE* fileLinkedLists, Travel* travel, int nextCur){
    fwrite(&travel->sourceid,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->dstid,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->hod,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->mean_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->standard_deviation_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->geometric_mean_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->geometric_standard_deviation_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&nextCur,sizeof(int),1,fileLinkedLists);
}

#endif