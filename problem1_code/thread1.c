//created by Kebin Li
//date 04/13/2020

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

int array_size = 1000000;
int max = 0;
int maxindex = 0;
int threadnum;

typedef struct{
    int thread_id;
    int* array;
}data;

void* func(void* arg){
    data *my_data = (data*)arg;
    int thread_id = my_data->thread_id;
    int *array = my_data->array;
    int i, chunksize;
    FILE *fptr;
    const char* filename = "result.txt";
    fptr = fopen(filename,"a");
    if (fptr == NULL)
    {
        printf("ERROR Creating File!");
        exit(1);
    }
    chunksize = array_size/threadnum;
    for(i = 0; i<chunksize; i++){
        if(array[thread_id*chunksize+i] == -50){
            fprintf(fptr, "Here is thread %d, and we find hidden value at index %d \n", thread_id, thread_id*chunksize+i);
        }else if(array[thread_id*chunksize+i]>max){
            max = array[thread_id*chunksize+i] ;
            maxindex = thread_id*chunksize+i;
        }
    }
    close(fptr);
    pthread_exit(0);
}

int main(int argc, char *argv[]){

    if(argc!=2){
        fprintf(stderr, "execute with 1 argument\n");
        exit(0);
    }

    threadnum = atoi(argv[1]);
    pthread_t thread[threadnum];
    clock_t t; 
    int chunksize = array_size/threadnum;
    FILE *fptr, *fd;
    const char* filename1 = "array.txt";
    const char* filename2 = "result.txt";
    fptr = fopen(filename1,"r");
    if (fptr == NULL)
    {
        printf("ERROR Reading File!");
        exit(1);
    }
    int array[array_size], number, i;
    i = 0;
    while(fscanf(fptr, "%d", &number)==1){
        array[i] = number;
        i++;
    }
    close(fptr);

    data mydata[threadnum];
    t = clock();
    for(i = 0; i<threadnum; i++){
        mydata[i].thread_id = i;
        mydata[i].array = array;
        pthread_create(&thread[i],NULL,&func, &mydata[i]);
    }
    for(i = 0; i<threadnum; i++){
        pthread_join(thread[i],NULL);
    }

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    fd = fopen(filename2,"a");
    fprintf(fd, "This is option1\n");
    fprintf(fd, "it took %f seconds to execute and we find maximum %d at %d \n\n", time_taken, max,maxindex);
    close(fd);
}