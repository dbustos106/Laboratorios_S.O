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

int main(){
    FILE *file;
    printf("Inicio programa\n");
    file=fopen("bogota-cadastral-2020-1-All-HourlyAggregate.txt", "rb");
    if(file==NULL){
        printf("Error leyendo archivo \n");
    }else{
        printf("Exito\n");
    }

    char* file_content = malloc(9*sizeof(char));

    int leidos = fscanf(file, "%[^,]", file_content);
    printf("%d", leidos);
    for(int i = 0; i < 9; i++){
        printf("EL elemento es: %c\n", *(file_content + i));
    }
    

    struct hashTable hash1;
    hash1.getHash = getHash;
    printf("Resultado: %d",hash1.getHash(1160));
    fclose(file);
    return 0;
}