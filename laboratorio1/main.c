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

    char* file_content = malloc(135*sizeof(char));
    int leidos = fscanf(file, "%[^\n]", file_content);
    
    printf("Leidos: %d\n", leidos);
    for(int i = 0; i < 135; i++){
        printf("%c", *(file_content + i));
    }
    printf("\n\n");

    double* file_content2 = malloc(9*sizeof(double));    
    char* comas = malloc(4*sizeof(char));
    for(int i = 0; i < 7; i++){
        fscanf(file, "%lf", file_content2 + i);
        fscanf(file, "%c", comas + i);
    }

    for(int i = 0; i < 7; i++){
        printf("EL elemento es: %lf\n", *(file_content2 + i));
    }
    
    


    
    free(file_content);
    free(file_content2);
    

    struct hashTable hash1;
    hash1.getHash = getHash;
    printf("Resultado: %d",hash1.getHash(1160));
    fclose(file);
    return 0;
}