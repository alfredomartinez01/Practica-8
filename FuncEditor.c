/* Funciones que usa el editor */
# include <ncurses.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h> 
# include <unistd.h> 
# include <time.h>
# include<sys/ipc.h>
# include<sys/shm.h>
#include <signal.h> 

/* Teclas para controlar al mono*/
# define ESCAPE 27
# define ENTER 10
# define DELETE 263

/* Declaracion de las vars principales */
WINDOW *Editor;  

/* Para obtener tamaño de ventanas*/
int maxX;
int maxY;

/* Variables para controlar al cursor y la informacion del texto*/
int posX, posY; // Posicion del cursor
int x, y; // Posicion del scroll horizontal y vertical 
char *direccionArchivo;
char *data[100]; /* 100 lineras por defecto*/
int lineasArchivo; // Las líneas que tendrá el archivo

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

    // Declaramos el arreglo para la lectura de todo el archivo
    for(int i=0; i < 100; i++){
        data[i] = (char*)malloc(10000*sizeof(char)); 
    }
    direccionArchivo = (char *) malloc(100*sizeof(char));

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

void mover(int posicionX, int posicionY){
    if((posX + posicionX >= 1) && (posX + posicionX <= maxX-2)) // limita el movimiento del mono para que no se salga del borde 
        posX = posX + posicionX; // Mueve la posicionX en n espacios dados

    if((posY + posicionY >= 2) && (posY + posicionY <= maxY-5)) // limita el movimiento del mono para que no se salga del borde 
        posY = posY + posicionY; // Mueve la posicionX en n espacios dados    
}

void comprobarScroll(int horizontal, int vertical){
    if(posX + horizontal < 1){ // Se dara scroll a la izquierda
        if(x > 0){
            x--;
        }
    }
    else if(posX + horizontal >= maxX-1){ // Se dara scroll a la izquierda
        
    }
}
