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

// colores
#define COLOR_TITULO      1
#define COLOR_CABALLO     2
#define COLOR_PISTA       3
#define COLOR_MENU        4
#define COLOR_GANADOR     5


// Macros para centrar el contenido vertical y horizontalmente
#define CENTER_Y (LINES / 2)
#define CENTER_X (COLS / 2)



// centrar todos los textos en el centro de la pantalla
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

void dibujar_pista(int cantidad_caballos,Carrera &carrera) {
    clear();
    attron(COLOR_PAIR(COLOR_TITULO) | A_BOLD);
    mvprintw(CENTER_Y - 5, CENTER_X - 10, "===== HIPODROMO =====");
    attroff(COLOR_PAIR(COLOR_TITULO) | A_BOLD);


    for (int i = 0; i < cantidad_caballos; i++){
        Caballo* caballo = (carrera.getCaballos() + i);

        attron(COLOR_PAIR(COLOR_PISTA) | A_BOLD);
        mvprintw(CENTER_Y - 3 + i, CENTER_X - 45, "Caballo %d: ", caballo->getId());
        attroff(COLOR_PAIR(COLOR_PISTA) | A_BOLD);

        for (int j = 0; j < carrera.getDistanciaPista(); j++) {
            if (j == caballo->getPosition()) {

                attron(COLOR_PAIR(COLOR_CABALLO) | A_BOLD);
                printw("C");
                attroff(COLOR_PAIR(COLOR_CABALLO) | A_BOLD);
            }
            else {
                attron(COLOR_PAIR(COLOR_CABALLO) | A_BOLD);
                printw("-");
                attroff(COLOR_PAIR(COLOR_CABALLO) | A_BOLD);
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
    mvprintw(CENTER_Y - 5, CENTER_X - 15, "===== RANKING DE CABALLOS =====");
    for (int i = 0; i < num_caballos; i++) {
        if (ranking_caballos[i] != 0) {

            // debe mostrar el id del caballo en el orden de posicion de ranking que se guardo en el arreglo ranking_caballos
            mvprintw(CENTER_Y - 3 + i, CENTER_X - 10, "%d. Caballo %d", i + 1, ranking_caballos[i]);
        }
    }
    mvprintw(CENTER_Y + 10, CENTER_X - 15, "Presione una tecla para continuar...");
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
    
    mvprintw(CENTER_Y + 2, CENTER_X - 15, "Carrera finalizada. Presione una tecla para continuar...");
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
        mvprintw(CENTER_Y - 2, CENTER_X - 20, "Digite el numero de caballos (2-7): ");
        scanw(formato, &num_caballos);

        if (num_caballos < 2 || num_caballos > 7) {
            mvprintw(CENTER_Y, CENTER_X - 15, "Cantidad invalida.");
            mvprintw(CENTER_Y + 1, CENTER_X - 15, "Presione una tecla para reintentar...");
            refresh();
            getch();
        }
    } while (num_caballos < 2 || num_caballos > 7);        

    do {
        clear();
        mvprintw(CENTER_Y - 1, CENTER_X - 20, "Digite el numero de vueltas (1-4): ");
        scanw(formato, &num_vueltas);

        if (num_vueltas < 1 || num_vueltas > 4) {
            mvprintw(CENTER_Y, CENTER_X - 15, "Cantidad invalida.");
            mvprintw(CENTER_Y + 1, CENTER_X - 15, "Presione una tecla para reintentar...");
            refresh();
            getch();
        }
    } while (num_vueltas < 1 || num_vueltas > 4);
    
    do {
        clear();
        mvprintw(CENTER_Y, CENTER_X - 20, "Digite la distancia de pista: ");
        scanw(formato, &num_d);
        if (num_d != 30 && num_d != 40 && num_d != 50 && num_d != 60) {
            mvprintw(CENTER_Y, CENTER_X - 15, "Cantidad invalida.");
            mvprintw(CENTER_Y + 1, CENTER_X - 15, "Presione una tecla para reintentar...");
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
        mvprintw(CENTER_Y - 5, CENTER_X - 20, "JUEGO DE CARRERA DE CABALLOS");
        mvprintw(CENTER_Y - 4, CENTER_X - 20, "1. Para iniciar el juego.");
        mvprintw(CENTER_Y - 3, CENTER_X - 20, "2. Salir.");
        mvprintw(CENTER_Y - 1, CENTER_X - 20, "Opcion: ");

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
                mvprintw(CENTER_Y, CENTER_X - 15, "Saliendo del programa");
                refresh();
                break;

            default:
                mvprintw(CENTER_Y + 3, CENTER_X - 15, "Opcion no disponible");
                mvprintw(CENTER_Y + 4, CENTER_X - 15, "Presione una tecla para continuar...");
                refresh();
                getch();
                break;
        }

    } while (op != 2);
}

int main() {

    initscr();

    start_color();
    use_default_colors();
    init_pair(COLOR_TITULO, COLOR_YELLOW, COLOR_BLUE);
    init_pair(COLOR_CABALLO, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PISTA, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_MENU, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_GANADOR, COLOR_YELLOW, COLOR_BLACK);

    
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    menu();

    endwin();

    return 0;
}