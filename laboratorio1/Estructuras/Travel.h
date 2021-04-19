#include<stdio.h>
#include<stdlib.h>

typedef struct Travel{
    int sourceid;
    int dstid;
    double hod;
    double mean_travel_time;
    double standard_deviation_travel_time;
    double geometric_mean_travel_time;
    double geometric_standard_deviation_travel_time;
}Travel;

void writeTravel(FILE* fileLinkedLists, int key, Travel* travel, int nextCur){
    fwrite(&key, sizeof(int),1,fileLinkedLists);
    fwrite(&travel->sourceid,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->dstid,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->hod,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->mean_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->standard_deviation_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->geometric_mean_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->geometric_standard_deviation_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&nextCur,sizeof(int),1,fileLinkedLists);
}

Travel* createTravel(){
    Travel *travel = (Travel*)malloc(sizeof(Travel));
    return travel;
}
