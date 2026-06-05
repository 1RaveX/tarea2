#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include "caballo.h"
#include "carrera.h"
#include "common.h"

#define NUM_HILOS 7
#define NUM_CABALLOS 7

pthread_mutex_t mutex_caballos = PTHREAD_MUTEX_INITIALIZER;
void* comportamiento_caballo(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    Caballo* caballo = data->caballo;
    Carrera* carrera = data->carrera;
    while (caballo->getVueltasActuales() < carrera->getNumVueltas()) {
        usleep(100000);
        int movimiento = randomMoveX();
        pthread_mutex_lock(&mutex_caballos);

        caballo->setPosition(caballo->getPosition() + movimiento);
        caballo->setMetrosTotales(caballo->getMetrosTotales() + movimiento);

        if (caballo->getPosition() >= carrera->getDistanciaPista()) {
            caballo->setVueltasActuales(caballo->getVueltasActuales() + 1);
    
            if(caballo->getVueltasActuales() >= carrera->getNumVueltas()) {
                
                caballo->setPosition(carrera->getDistanciaPista()-1);
                carrera->registrar_ranking(caballo->getId());

            } else {
                caballo->setPosition(0);
            }
        }
        pthread_mutex_unlock(&mutex_caballos);
    }
    pthread_exit(NULL);
}

void dibujar_pista(int cantidad_caballos,Carrera carrera) {
    clear();
    mvprintw(0,0,"===== HIPODROMO =====");

    for (int i = 0; i < cantidad_caballos; i++){
        Caballo* caballo = (carrera.getCaballos() + i);
        mvprintw(i + 2,0,"Caballo %d: ",caballo->getId());
        
        for (int j = 0;j < carrera.getDistanciaPista();j++) {
            if (j == caballo->getPosition()) {
                printw("C");
            }
            else {
                printw("-");
            }
        }
        printw(
            " | Vueltas: %d/%d | Metros: %d",
            caballo->getVueltasActuales(),
            carrera.getNumVueltas(),
            caballo->getMetrosTotales()
        );
    }
    refresh();
}

void mostrar_resultado_ranking(int *ranking_caballos, int num_caballos) {
    clear();
    mvprintw(1, 1, "===== RANKING DE CABALLOS =====");
    for (int i = 0; i < num_caballos; i++) {
        if (ranking_caballos[i] != 0) {
            mvprintw(i + 3, 1, "%d. Caballo %d", i + 1, (*ranking_caballos + i));
        }
    }
    mvprintw(num_caballos + 5, 1, "Presione una tecla para continuar...");
    refresh();
    getch();
}

void iniciar_carrera(Carrera &carrera,Caballo caballos[],int num_caballos) {
    pthread_t threads[NUM_CABALLOS];
    ThreadData data[NUM_CABALLOS];

    for (int i = 0; i < num_caballos; i++) {
        data[i].caballo = &caballos[i];
        data[i].carrera = &carrera;
    }

    createThread(
        threads,
        data,
        num_caballos,
        comportamiento_caballo
    );

    bool carrera_finalizada = false;

    while (!carrera_finalizada) {
        clear();
        dibujar_pista(num_caballos, carrera);
        refresh();
        usleep(100000);
        carrera_finalizada = true;
        for (int i = 0; i < num_caballos; i++) {
            if (caballos[i].getVueltasActuales() < carrera.getNumVueltas()) {
                carrera_finalizada = false;
                break;
            }
        }
    }

    runThread(threads, num_caballos);
    clear();
    dibujar_pista(num_caballos, carrera);
    refresh();
    
    mvprintw(num_caballos + 5,1,"Carrera finalizada. Presione una tecla para continuar...");
    mostrar_resultado_ranking(carrera.getRankingCaballos(), num_caballos);
    refresh();
    getch();
}



// preparar_carrera
void preparar_carrera() {
    int num_vueltas;
    int num_caballos;
    int num_d;
    char formato[] = "%d";

    clear();

    echo();
    curs_set(TRUE);

    do {
        clear();
        mvprintw(1, 1, "Digite el numero de caballos (2-7): ");
        scanw(formato, &num_caballos);

        if (num_caballos < 2 || num_caballos > 7) {
            mvprintw(2, 1, "Cantidad invalida.");
            mvprintw(3, 1, "Presione una tecla para reintentar...");
            refresh();
            getch();
        }
    } while (num_caballos < 2 || num_caballos > 7);        

    do {
        clear();
        mvprintw(1, 1, "Digite el numero de vueltas (1-4): ");
        scanw(formato, &num_vueltas);

        if (num_vueltas < 1 || num_vueltas > 4) {
            mvprintw(2, 1, "Cantidad invalida.");
            mvprintw(3, 1, "Presione una tecla para reintentar...");
            refresh();
            getch();
        }
    } while (num_vueltas < 1 || num_vueltas > 4);
    
    do {
        clear();
        mvprintw(1, 1, "Digite la distancia de pista: ");
        scanw(formato, &num_d);
        if (num_d != 30 && num_d != 40 && num_d != 50 && num_d != 60) {
            mvprintw(2, 1, "Cantidad invalida.");
            mvprintw(3, 1, "Presione una tecla para reintentar...");
            refresh();
            getch();
        }
    } while (num_d != 30 && num_d != 40 && num_d != 50 && num_d != 60);


    noecho();
    curs_set(FALSE);
    Caballo caballos[NUM_CABALLOS];

    for (int i = 0; i < num_caballos; i++) {
        caballos[i].setId(i + 1);
        caballos[i].setPosition(0);
        caballos[i].setVueltasActuales(0);
        caballos[i].setMetrosTotales(0);
    }

    Carrera carrera(num_vueltas, num_d, caballos);
    iniciar_carrera(carrera, caballos, num_caballos);
}




void menu() {
    int op;
    char formato[] = "%d";


    do {
        clear();
        mvprintw(1, 1, "JUEGO DE CARRERA DE CABALLOS");
        mvprintw(2, 1, "1. Para iniciar el juego.");
        mvprintw(3, 1, "2. Salir.");
        mvprintw(5, 1, "Opcion: ");

        echo();
        scanw(formato, &op);
        noecho();

        switch (op) {
            case 1:
                preparar_carrera();
                clear();
                refresh();
                break;

            case 2:
                clear();
                mvprintw(1, 1, "Saliendo del programa");
                refresh();
                break;

            default:
                mvprintw(7, 1, "Opcion no disponible");
                mvprintw(8, 1, "Presione una tecla para continuar...");
                refresh();
                getch();
                break;
        }

    } while (op != 2);
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    menu();

    endwin();

    return 0;
}