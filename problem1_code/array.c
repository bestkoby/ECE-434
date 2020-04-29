//created by Kebin Li
//date 04/13/2020

#include<stdio.h>
#include<stdlib.h>

int size = 1000000;

int main(){
    FILE *fptr;
    const char* filename = "array.txt";
    fptr = fopen(filename,"w");
    if (fptr == NULL)
    {
        printf("ERROR Creating File!");
        exit(1);
    }
    int i;
    int a = -50;
    int b = 100;
    int max = rand()%size;
    if (max == size/4-1||max == size/2-1||max == 3*size/4-1){
        max = rand()%size;
    }
    for (i = 0; i<size; i++){
        if(i==max){
            fprintf(fptr, "%d ", b);
        }else if(i!=size/4-1&&i!=size/2-1&&i!=3*size/4-1){
            fprintf(fptr, "%d ", rand()%100);
        }else{
            fprintf(fptr, "%d ", a);
        }
    }
    close(fptr);
}