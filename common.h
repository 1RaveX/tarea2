#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

class Caballo;

struct Carrera {
    int num_vueltas;
    int distancia_pista;
    Caballo* caballos;
};

struct ThreadData {
    Caballo* caballo;
    Carrera* carrera;
};

typedef void* (*callback)(void*);

int randomMoveX();

void createThread(
    pthread_t* threads,
    ThreadData* data,
    const int& size_thread,
    callback func
);

void runThread(
    pthread_t* threads,
    const int& hilosN
);

#endif