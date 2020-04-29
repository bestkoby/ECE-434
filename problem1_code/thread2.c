//created by Kebin Li
//date 04/13/2020

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>


int array_size = 1000000;
int max = 0;
int maxindex = 0;

typedef struct{
    //int thread_id;
    int* array;
    int startindex;
    int chunksize;
}data;

void* func(void* arg){
    data *my_data = (data*)arg;
    int chunksize = my_data->chunksize;
    int *array = my_data->array;
    int startindex = my_data->startindex;
    if(chunksize>10000){
        pthread_t thread1, thread2;
        data mydata1, mydata2;
        mydata1.array = array;
        mydata1.startindex = startindex;
        mydata1.chunksize = chunksize/2;
        mydata2.array = array;
        mydata2.startindex = startindex+chunksize/2;
        if (chunksize%2 == 0){
            mydata2.chunksize = chunksize/2;
        }else{
            mydata2.chunksize = chunksize/2+1;
        }
        pthread_create(&thread1,NULL,&func, &mydata1);
        pthread_create(&thread2,NULL,&func, &mydata2);
        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
    }else{
    int i;
    FILE *fptr;
    const char* filename = "result.txt";
    fptr = fopen(filename,"a");
    if (fptr == NULL)
    {
        printf("ERROR Creating File!");
        exit(1);
    }
    for(i = 0; i<chunksize; i++){
        if(array[startindex+i] == -50){
            fprintf(fptr, "Here is thread unknown, and we find hidden value at index %d \n", startindex+i);
        }else if(array[startindex+i]>max){
            max = array[startindex+i] ;
            maxindex = startindex+i;
        }
    }
    close(fptr);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    clock_t t; 
    pthread_t thread;
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
    t = clock();
    data mydata;
    mydata.array = array;
    mydata.startindex = 0;
    mydata.chunksize = array_size;
    pthread_create(&thread,NULL,&func, &mydata);
    pthread_join(thread,NULL);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    fd = fopen(filename2,"a");
    fprintf(fd, "This is option2\n");
    fprintf(fd, "it took %f seconds to execute and we find maximum %d at %d \n\n", time_taken, max,maxindex);
    close(fd);
}