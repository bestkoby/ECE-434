//created by Kebin Li
//date 04/14/2020

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stddef.h>
#include<features.h>

void sig_handler(int signum){
    printf("Here is sighandler catching signum %d \n", signum); 
}

void sig_handler1(int signum){
    printf("Here is thread 1 sighandler catching signum %d \n", signum); 
}

void sig_handler2(int signum){
    printf("Here is thread 2 sighandler catching signum %d \n", signum); 
}

void sig_handler3(int signum){
    printf("Here is thread 3 sighandler catching signum %d \n", signum); 
}

void* func(void* arg){
    int thread_id = *((int*)arg);
    int result = 0;
    int a;
    for(a = 0; a<=1000*thread_id; a++){
        result = result + a;
    }
    struct sigaction newaction1, newaction2, newaction3;
    sigset_t mywait_mask;
    if(thread_id == 1){
        newaction1.sa_handler = sig_handler1;
        sigaction(SIGINT, &newaction1, NULL);
        sigaddset(&newaction1.sa_mask, SIGILL);
        sigaddset(&newaction1.sa_mask, SIGSTOP);
        if(sigprocmask(SIG_BLOCK, &newaction1.sa_mask, NULL)<0){
        fprintf(stderr, "sigblocking error at thread %d \n", thread_id);
        }
      //  printf("Here is thread %d starting accepting signal \n", thread_id); 
    }else if(thread_id == 2){
        newaction2.sa_handler = sig_handler2;
        sigaction(SIGSTOP, &newaction2, NULL);
        sigaddset(&newaction2.sa_mask, SIGINT);
        sigaddset(&newaction2.sa_mask, SIGILL);
        if(sigprocmask(SIG_BLOCK, &newaction2.sa_mask, NULL)<0){
        fprintf(stderr, "sigblocking error at thread %d \n", thread_id);
        }
      //  printf("Here is thread %d starting accepting signal \n", thread_id); 
    }else if(thread_id == 3){
        newaction3.sa_handler = sig_handler3;
        sigaction(SIGILL, &newaction3, NULL);
        sigaddset(&newaction3.sa_mask, SIGINT);
        sigaddset(&newaction3.sa_mask, SIGSTOP);
        if(sigprocmask(SIG_BLOCK, &newaction3.sa_mask, NULL)<0){
        fprintf(stderr, "sigblocking error at thread %d \n", thread_id);
        }
        //printf("Here is thread %d starting accepting signal \n", thread_id); 
    }

    sleep(20);
    /*
    sigpending(&mywait_mask);
    if (sigismember(&mywait_mask,SIGINT)) {fprintf(stderr,"Block SIGINT at thread %d\n", thread_id);}
    if (sigismember(&mywait_mask,SIGSTOP)) {fprintf(stderr,"Block SIGSTOP at thread %d\n", thread_id);}
    if (sigismember(&mywait_mask,SIGILL)) {fprintf(stderr,"Block SIGILL at thread %d\n", thread_id);}
    */
    printf("Here is thread %d, and i got sum is %d \n", thread_id, result);
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    printf("my pid is %d\n", getpid());
    int i;
    pthread_t thread[3];
    sigset_t mainmask, waiting_mask;
    struct sigaction myaction;
    myaction.sa_handler = sig_handler;
    sigaction(SIGINT, &myaction, NULL);
    sigaction(SIGSTOP, &myaction, NULL);
    sigaction(SIGILL, &myaction, NULL);
    sigaddset(&myaction.sa_mask, SIGINT);
    sigaddset(&myaction.sa_mask, SIGILL);
    sigaddset(&myaction.sa_mask, SIGSTOP);
    if(sigprocmask(SIG_BLOCK, &myaction.sa_mask, NULL)<0){
        fprintf(stderr, "sigblocking error!");
    }
    printf("process start received signal\n"); 
    sleep(8);
    sigpending(&waiting_mask);
    if (sigismember(&waiting_mask,SIGINT)) {fprintf(stderr,"User tried to kill parent\n");}
    if (sigismember(&waiting_mask,SIGSTOP)) {fprintf(stderr,"SIGSTOP was send while parent blocked\n");}
    if (sigismember(&waiting_mask,SIGILL)) {fprintf(stderr,"SIGILL was send while parent blocked\n");}
    int threadnum[3];
    printf("create threading \n"); 
    for(i = 1; i<4; i++){
        threadnum[i-1] = i;
        pthread_create(&thread[i-1],NULL,&func, &threadnum[i-1]);
    }
    
    
    if(sigprocmask(SIG_UNBLOCK, &myaction.sa_mask, NULL)<0){
        fprintf(stderr, "unblocking error!");
    }
    

    for(i = 1; i<4; i++){
        pthread_join(thread[i-1],NULL);
    }
    
    myaction.sa_handler = SIG_DFL;
    printf("i am done\n");
}