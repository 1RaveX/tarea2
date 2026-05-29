#include<iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include "caballo.h"
#include "common.h"

#define NUM_HILOS 5
#define NUM_CABALLOS 7


void* comportamiento_caballo(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    Caballo* caballo = data->caballo;
    Carrera* carrera = data->carrera;

    while (caballo->getVueltasActuales() < carrera->num_vueltas ) {
        usleep(200000);

        int movimiento = randomMoveX();

        caballo->setPosition(
            caballo->getPosition() + movimiento
        );

        caballo->setMetrosTotales(
            caballo->getMetrosTotales() + movimiento
        );

        if (caballo->getPosition() >= carrera->distancia_pista) {
            caballo->setPosition(0);

            caballo->setVueltasActuales(
                caballo->getVueltasActuales() + 1
            );
        }
    }

    pthread_exit(NULL);
}

void dibujar_pista(int cantidad_caballos,Carrera carrera) {
    clear();
    mvprintw(0,0,"===== HIPODROMO =====");

    for (int i = 0; i < cantidad_caballos; i++){
        Caballo* caballo = (carrera.caballos + i);
        mvprintw(i + 2,0,"Caballo %d: ",caballo->getId());

        for (int j = 0;j < carrera.distancia_pista;j++) {
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
            carrera.num_vueltas,
            caballo->getMetrosTotales()
        );
    }

    refresh();
}


// preparar_carrera
void preparar_carrera() {
    int num_vueltas, num_caballos, num_d;

    std::cout << "Digite el numero de caballos: ";
    std::cin >> num_caballos;

    if (num_caballos < 2 || num_caballos > 7) {
        std::cout << "Cantidad invalida\n";
        return;
    }

    std::cout << "Digite el numero de vueltas: ";
    std::cin >> num_vueltas;

    std::cout << "Digite la distancia de pista: ";
    std::cin >> num_d;

    Caballo caballos[NUM_CABALLOS];

    for (int i = 0; i < num_caballos; i++)
    {
        caballos[i].setId(i + 1);
        caballos[i].setPosition(0);
        caballos[i].setVueltasActuales(0);
        caballos[i].setMetrosTotales(0);
    }

    Carrera carrera = {
        num_vueltas,
        num_d,
        caballos
    };
    pthread_t threads[NUM_CABALLOS];
    ThreadData data[NUM_CABALLOS];

    for (int i = 0; i < num_caballos; i++) {
        data[i].caballo = &caballos[i];
        data[i].carrera = &carrera;
    }

    initscr();
    noecho();
    curs_set(FALSE);

    createThread(
        threads,
        data,
        num_caballos,
        comportamiento_caballo
    );

    bool carrera_finalizada = false;

    while (!carrera_finalizada) {
        dibujar_pista(
            num_caballos,
            carrera
        );

        usleep(100000);

        carrera_finalizada = true;

        for (int i = 0; i < num_caballos; i++)
        {
            if (
                caballos[i].getVueltasActuales() <
                carrera.num_vueltas
            )
            {
                carrera_finalizada = false;
                break;
            }
        }
    }

    runThread(
        threads,
        num_caballos
    );

    getch();
    endwin();

}

// paresentar resultado
void presentar_resultado() {

}



void renderizado_carrera() {
    // tamaño maximo width
    initscr();
    refresh();
    int WIDTH_2 = COLS/8;
    int caballos_tam = 5;

    mvprintw(0,WIDTH_2, "CARRERA");
    for(int i=1;i<=caballos_tam;++i) {
        mvprintw(i, 0, "CABALLO: %s, VUELTA %d - %d metros: ", "Caballo 1", 3, 300);    
    }
    mvprintw(8, 0, "Totales: %d VUELTAS  - %d metros: ", 10, 1000);
    int acc = 8;
    for(int i=0;i<7;++i) {
        int row = i+acc;
        mvprintw(row, 0, "CARRIL: %d | %c | -- | N", i+1, 'A');    
    }
    mvprintw(8+acc, WIDTH_2, "DISTANCIA");
    
    getch();
    endwin();
}



void menu() {
    int op;
    do {
        std::cout << "JUEGO DE CARRERA DE CABALLOS" << std::endl;
        std::cout << "1. Para inciar el juego." << std::endl;
        std::cout << "2. salir. " << std::endl;
        std::cout << "Opcion: "; std::cin>> op;

        switch (op) {
        case 1:
            preparar_carrera();
            printf("\033[H\033[2J");
            fflush(stdout);
            break;
        case 2:
            std::cout << "Saliendo del programa" << std::endl;
            break;
        default:
            std::cout << "Opcion no disponible" << std::endl;
            break;
        }
    } while (op != 2);

}


int main() {
    printf("\033[H\033[2J");
    fflush(stdout);

    std::cout << "Iniciando" << std::endl;
    menu();

    return 0;
}