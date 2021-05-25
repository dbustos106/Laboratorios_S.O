#ifndef Queue_h
#define Queue_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"

typedef struct ClientEstruct{
    int clientfd;
    char* clientIP;
}ClientEstruct;

Node* head = NULL;
Node* tail = NULL;   

void enqueue(ClientEstruct* clientp){
    Node* newNode = malloc(sizeof(Node));
    newNode->clientp = clientp;
    newNode->next = NULL;
    if(tail == NULL){
        head = newNode;
    }else{
        tail->next = newNode;
    }
    tail = newNode;
}

ClientEstruct* dequeue(){
    if(head == NULL){
        return NULL;
    }else{
        ClientEstruct* clientp = head->clientp;
        Node* temp = head;
        head = head->next;
        if(head == NULL){
            tail = NULL;
        }
        free(temp);
        return clientp;
    }
}

#endif
