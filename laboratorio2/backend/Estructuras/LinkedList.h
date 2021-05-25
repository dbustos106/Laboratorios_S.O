#ifndef LinkedList_h
#define LinkedList_h

#include<stdio.h>
#include<stdlib.h>

typedef struct LinkedList{
    int headCur;
    int tailCur;
}LinkedList;

LinkedList initList(){
    LinkedList linkedList;
    linkedList.headCur = -1;
    linkedList.tailCur = -1; 
    return linkedList;
}

void createList(LinkedList *list, int headCur, int tailCur) {
    list->headCur = headCur;
    list->tailCur = tailCur;
}

#endif