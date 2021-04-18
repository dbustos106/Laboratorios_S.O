#include<stdio.h>
#include<stdlib.h>

typedef struct Travel{
    double sourceid;
    double dstid;
    double hod;
    double mean_travel_time;
    double standard_deviation_travel_time;
    double geometric_mean_travel_time;
    double geometric_standard_deviation_travel_time;
}Travel;

Travel* createTravel(){
    Travel *travel = malloc(sizeof(Travel));
    /*
    linkedList->sourceid = NULL;
    linkedList->dstid = NULL; 
    linkedList->hod = 0 = ;
    linkedList->mean_travel_time;
    linkedList->standard_deviation_travel_time;
    linkedList->geometric_mean_travel_time;
    linkedList->geometric_standard_deviation_travel_time;
    */
   return travel;
}
