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

    move(2, 1);
    refresh();
    return 0;
}

int tamanoLinea(int linea){ /* Algoritmo para conoces cuantos caracteres tiene la linea */
    char *aux = (char *)malloc(10000*sizeof(char));
    strcpy(aux, data[linea]); // Mantenemos en memoria la linea antes del cambio
    int i = 0;
    while(*aux != '\0' && *aux != '\n'){
        i++;
        aux++;
    }
    return i;
}

int mover(int posicionX, int posicionY){
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
    else if(posX + horizontal >= maxX-1){ // Se dara scroll a la derecha
        x++;
    }
    
    if(posY + vertical < 2){ // Se dara scroll hacia arriba
        if(y > 0){
            y--;
        }
    }
    else if(posY + vertical >= maxY-4){ // Se dara scroll hacia abajo
        if(lineasArchivo - y >  maxY-6){
            y++;
        }
        
    }
}

void eliminarCaracter(){   

    char nueva_linea[100000] ="";  // Crea una línea para después reasignarla a la original
    char *aux = (char *)malloc(10000*sizeof(char));
    strcpy(aux, data[(posY-2)+y]); // Mantenemos en memoria la linea antes del cambio
    mover(-1, 0);
    
    /* Algoritmo de eliminación del caracter de la posición del cursor sobre la cadena ((posX-1)+x)*/
    int i = 0;
    while(*aux != '\0'){
        if(i == (posX-1)+x){
            aux++;
        }
        nueva_linea[i] = *aux;
        aux++;
        i++;
    }
    strcpy(data[(posY-2)+y], nueva_linea);
}

void insertarCaracter(char caracter){   

    char nueva_linea[100000] ="";  // Crea una línea para después reasignarla a la original
    char *aux = (char *)malloc(10000*sizeof(char));
    strcpy(aux, data[(posY-2)+y]); // Mantenemos en memoria la linea antes del cambio

    
    /* Algoritmo de insersión del caracter en la posición del cursor sobre la cadena ((posX-1)+x)*/
    int i = 0;
    while(*aux != '\0'){
        if(i == (posX-1)+x){
            nueva_linea[i] = caracter;
            i++;
        }
        nueva_linea[i] = *aux;
        aux++;
        i++;
    }
    strcpy(data[(posY-2)+y], nueva_linea);
    mover(1, 0);    
}
