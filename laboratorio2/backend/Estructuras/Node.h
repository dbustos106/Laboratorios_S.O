#ifndef Node_h
#define Node_h

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Node{
    struct Node* next;
    struct ClientEstruct* clientp;
}Node;

#endif