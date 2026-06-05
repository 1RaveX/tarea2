#ifndef COMMON_H
#define COMMON_H
#include <pthread.h>

class Caballo;
class Carrera;

typedef struct {
    Caballo* caballo;
    Carrera* carrera;
} ThreadData;
typedef void* (*callback)(void*);

int randomMoveX();
void createThread(pthread_t* threads,ThreadData* data,const int& size_thread,callback func);
void runThread(pthread_t* threads,const int& hilosN);

#endif