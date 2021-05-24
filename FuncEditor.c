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

void subirLineas(int linea){
    for(int i = linea; i <= lineasArchivo; i++){
        data[i] = data[i+1];
    }
}
void bajarLineas(int linea){
    for(int i = lineasArchivo; i > linea; i--){
        strcpy(data[i], data[i-1]);
    }
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

void insertarCaracter(char caracter){   

    char nueva_linea[100000] = "";  // Crea una línea para después reasignarla a la original
    char *aux = (char *)malloc(10000*sizeof(char));
    strcpy(aux, data[(posY-2)+y]); // Mantenemos en memoria la linea antes del cambio

    
    /* Algoritmo de insersión del caracter en la posición del cursor sobre la cadena ((posX-1)+x)*/
    if(tamanoLinea((posY-2)+y) > 0){
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
        aux--;
    }
    else{
        nueva_linea[0] = caracter;
        nueva_linea[1] = '\0';
    }
    
    strcpy(data[(posY-2)+y], nueva_linea);
    
    if( ((posX-1)+x) + 1 <= tamanoLinea((posY-2)+y)){ // Comprueba que el tamaño de linea permita moverlo
        mover(1, 0);
        comprobarScroll(1, 0);
    }
}

void saltoLinea(){
    char linea_original[10000] = "";  // Crea una línea para después reasignarla a la original
    char linea_nueva[10000] = "";  // Crea una línea para después reasignarla al siguiente renglon

    char aux[100000];
    strcpy(aux, data[(posY-2)+y]); // Mantenemos en memoria la linea antes del cambio

    /*Bajamos el resto de lineas*/
    lineasArchivo++;
    bajarLineas((posY-2)+ y+1);
    strcpy(data[(posY-2)+y + 1], "");

    // Algoritmo de insersión de salto de linea en la posicion del cursor (posX-2)+x
    // Partimos en dos
    for(int i=0; i <= (posX-2)+x; i++){ // Para obtener la primer linea
        linea_original[i] = aux[i];   
        if(i == (posX-2)+x){
            linea_original[i+1] = '\n';
        }  
    }
    int j= 0;
    for(int i= (posX-2)+x +1; i <= tamanoLinea((posY-2)+ y); i++){ // Para obtener la segunda linea
        linea_nueva[j] = aux[i];    
        j++;
    }
    
    strcpy(data[(posY-2)+y], linea_original);
    strcpy(data[(posY-2)+y+1], linea_nueva);
    
    // Hacemos retorno de carro en la siguiente linea
    if( (posY-2)+y + 1 < lineasArchivo){  // Comprueba que la linea siguiente sí sea una linea existente              
        mover(0, 1);
        comprobarScroll(0, 1);
        x = 0;
        posX = 1;        
    }
}

void eliminarSalto(){
    char linea_original[10000] = "";  // Crea una línea para después reasignarla a la original
    char linea_nueva[10000] = "";  // Crea una línea para después reasignarla al siguiente renglon

    char aux[100000];
    strcpy(aux, data[(posY-2)+y]); // Mantenemos en memoria la linea antes del cambio

    /*subimos el resto de lineas*/
    subirLineas((posY-2)+ y);
    lineasArchivo--;

    // Algoritmo de concatenación de lineas por eliminación de salto  
    
    strcpy(data[(posY-2)+y-1], data[(posY-2)+y]);
    strcpy(data[(posY-2)+y], "");
    
    // Hacemos retorno de carro en la siguiente linea
    if( (posY-2)+y + 1 < lineasArchivo){  // Comprueba que la linea siguiente sí sea una linea existente              
        mover(0, 1);
        comprobarScroll(0, 1);
        x = 0;
        posX = 1;        
    }
}


void eliminarCaracter(){   
    if((posX-1)+x > 0){ // Si hay algo que eliminar
        if(posX-1 > 0){ // Si el cursor está adelante de la posición inicial
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
        else{ // Si el cursor está en el borde izquierdo
            char nueva_linea[100000] ="";  // Crea una línea para después reasignarla a la original
            char *aux = (char *)malloc(10000*sizeof(char));
            strcpy(aux, data[(posY-2)+y]); // Mantenemos en memoria la linea antes del cambio
            posX -=1 ;
            
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
            posX+=1;
            strcpy(data[(posY-2)+y], nueva_linea);
            comprobarScroll(-1, 0);
        }
        
    }
    else{
        if(posY-2 > 0 || y > 0){ // Debe haber algun scroll hacia abajo o no ser la primera linea
            if (lineasArchivo > 1 && tamanoLinea((posY-2)+y-1) == 0){ // Para subir el cursor
                subirLineas((posY-2)+y-1);
                lineasArchivo--;
                mover(0, -1);
                comprobarScroll(0, -1);
            }   
            else if (lineasArchivo > 1 && tamanoLinea((posY-2)+y-1) > 0){ // Para subir el texto 
                // Debemos eliminar el salto de linea y juntar las lineas
                eliminarSalto();
            } 
        }
            
    }
}
