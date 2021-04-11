#include<stdio.h>
#include<stdlib.h>

int p = 6007;
int x = 29;
int m;
int n;

struct node{
    int id;
    struct node *next;
    double data[8];    
};

struct linkedList{
    struct node *head;
    struct node *tail;
    int size;
};

typedef int(*GetHash)(int a);

struct hashTable{
    struct linkedList array[1];
    GetHash getHash;
};

int getHash (int id) {
    return id;
}

FILE* cargarDatos(){
    FILE *file;
    printf("Inicio programa");
    file=fopen("bogota-cadastral-2020-1-All-HourlyAggregate.txt", "r");
    if(file==NULL){
        printf("Error leyendo archivo \n");
    }else{
        printf("Exito");
    }
    fclose(file);
    return file;
}

int main(){
    FILE* file = cargarDatos();
    char primerDato;
    fscanf(file, "%c", &primerDato);
    printf(" d:  %c",primerDato);
    char primerDato2;
    fscanf(file, "%c", &primerDato2);
    printf(" d:  %c",primerDato2);
    struct hashTable hash1;
    hash1.getHash = getHash;
    printf("Resultado: %d",hash1.getHash(1160));
    return 0;
}