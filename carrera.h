#ifndef CARRERA_H
#define CARRERA_H

#include "caballo.h"

#define NUM_HILOS 7
#define NUM_CABALLOS 7

class Carrera {
private:
    int num_vueltas;
    int distancia_pista;
    Caballo* caballos;

    int ranking_caballos[NUM_CABALLOS];
    int ultima_posicion_ranking =0;

public:
    Carrera(int num_vueltas_, int distancia_pista_, Caballo* caballos_)
        : num_vueltas(num_vueltas_), distancia_pista(distancia_pista_), caballos(caballos_) {};
    ~Carrera() {};

    int getNumVueltas() const { return num_vueltas; }
    void setNumVueltas(int n) { num_vueltas = n; }

    int getDistanciaPista() const { return distancia_pista; }
    void setDistanciaPista(int d) { distancia_pista = d; }

    Caballo* getCaballos() const { return caballos; }
    void setCaballos(Caballo* c) { caballos = c; }
    void registrar_ranking(int id_caballo);

    int * getRankingCaballos() { return ranking_caballos; }

};

#endif