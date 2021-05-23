/* Funciones que usa el editor */
# include <ncurses.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h> 
# include <unistd.h> 
# include <time.h>
# include<sys/ipc.h>
# include<sys/shm.h>
#include <signal.h> 

/* Teclas para controlar al mono*/
# define ESCAPE 27
# define ENTER 10
# define IZQUIERDA 97
# define DERECHA 100
# define ARRIBA 119
# define ABAJO 115

/* Declaracion de las vars principales */
WINDOW *Editor;  

/* Para obtener tamaño de ventanas*/
int maxX;
int maxY;

/* Variables para controlar al cursor*/
int posX, posY; 

/* c�digo implementaci�n de todas las funciones */
void crearVentana(){
    Editor = initscr(); // Crea la ventana
    //inicializacion de colores
    if(has_colors())
	    start_color();

    init_pair(1,COLOR_BLACK, COLOR_WHITE); //texto
    init_pair(2,COLOR_BLUE,COLOR_WHITE); //mono
    init_pair(3,COLOR_MAGENTA, COLOR_MAGENTA); //cubos
    bkgd(COLOR_PAIR(1));
    attron(COLOR_PAIR(1));

    // Obtenemos resolución en espacios y renglones
	maxY = getmaxy(Editor);
	maxX = getmaxx(Editor); 
    posX = (maxX/2)-2;
    posY = maxY-6;
}

/* Función encargada de mostrar el header y el footer del editor*/
int mostrarHeaderFooter(){
    clear();
    attron(A_BOLD);
    //attron(A_BLINK);

    /* Mostrando titulo*/
    move(0, maxX/2-5);
    printw("Editron");

    for(int i = 0; i<maxX; i++){
        move(1, i);
        printw("-");
    }
    
    /* Mostrando footer*/
    for(int i = 0; i<maxX; i++){
        move(maxY-4, i);
        printw("-");
    }

    move(maxY-3, 1);
    printw("Ctrl + g para guardar");
    move(maxY-2, 1);
    printw("Ctrl + o para volver a menu");

    move(maxY-3, 30);
    printw("Ctrl + c para para salir");
    move(maxY-2, 30);
    printw("Ctrl + o para volver a menu");

    refresh();
    return 0;
}

void mover(int posicion){
    if((posX + posicion >= 0) && (posX + posicion <= maxX-5)) // limita el movimiento del mono para que no se salga del borde 
        posX = posX + posicion; // Mueve la posicion en n espacios dados
}

void dibujar(){
    /* Pintura del monito */
    attron(COLOR_PAIR(2));    
    move(posY, posX);
    printw("  O");

    refresh();
}

