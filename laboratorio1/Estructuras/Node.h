#ifndef Node_h
#define Node_h

#include<stdio.h>
#include<stdlib.h>
#include"Travel.h"

typedef struct Node{
    int key;
    struct Travel travel;  
    struct Node* next;
}Node;

Node* createNode(int key, Travel *travel){
    Node* node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->next = NULL; 
    node->travel = *travel;
    return node;
}

void eliminarNodo(Node* node){
    free(node);
}

void escribirNodo(FILE* fileLinkedLists, Node* node){
    fprintf(fileLinkedLists, "%d,%d,%d,%f,%f,%f,%f,%f,%p\n", node->key, 
        node->travel.sourceid, 
        node->travel.dstid,
        node->travel.hod,
        node->travel.mean_travel_time,
        node->travel.standard_deviation_travel_time,
        node->travel.geometric_mean_travel_time,
        node->travel.geometric_standard_deviation_travel_time,
        node->next);
}

#endif