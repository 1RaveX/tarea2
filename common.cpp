#include <cstdlib>
#include <cstdio>
#include <pthread.h>

#include "common.h"
#include "caballo.h"

int randomMoveX() { 
    return rand() % 3;
}

void createThread(pthread_t* threads,ThreadData* data,const int& size_thread,callback func) {
    for (int i = 0; i < size_thread; i++) {
        pthread_create(threads + i,NULL,func,&data[i]);
    }
}

void runThread(pthread_t* threads,const int& hilosN) {
    for (int i = 0; i < hilosN; i++) {
        pthread_join(threads[i], NULL);
    }
}