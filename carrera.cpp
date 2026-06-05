#include "carrera.h"

void Carrera::registrar_ranking(int id_caballo) {
    if(ultima_posicion_ranking < NUM_CABALLOS) {
        ranking_caballos[ultima_posicion_ranking++] = id_caballo;
    }
}