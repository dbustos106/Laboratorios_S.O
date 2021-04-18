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

Travel* createTravel(){
    Travel *travel = (Travel*)malloc(sizeof(Travel));
   return travel;
}
