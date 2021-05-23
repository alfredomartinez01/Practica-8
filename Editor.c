/* Interfaz editor*/
#include "FuncEditor.c"

/* Funcion dedicada a la lectura del archivo y almacenamiento en memoria*/
void lectura(){
    FILE *archivo = fopen(direccionArchivo, "r");
    lineasArchivo = 0;

    while(feof(archivo) == 0){ // Recorremos cada estructura del archivo
        fgets(data[lineasArchivo], 10000, archivo);
        lineasArchivo++;
    }
    fclose(archivo);
}

/* Función dedicada a imprimir todo el texto en pantalla*/
void imprimeTexto(){
    move(posY, posX);

    for(int espY = posY; espY<= maxY-5; espY++){ // No se debe desbordar horizontalmente
        /* Obtenemos una copia*/
        char *aux = data[y + (espY-posY)];

        // Para dejar el puntero en la posicion correcta en x (para scroll)
        for(int i=0; i <x; i++){
            aux++;
        }

        for(int espX = posX; espX <= maxX-2; espX++){ // No se debe desbordar verticalmente

            if(*aux != 00){
                move(espY, espX);
                printw("%c", *aux);
                aux++;
            }            
        }
    }
}


int abrirArchivo (char *direccion){
    /* Creamos ventana y sus elementos*/
    crearVentana();
    mostrarHeaderFooter();
    /* Donde partira a imprimir respecto a la ventana*/
    posX = 1;
    posY = 2;
    /* Donde partira a imprimir respecto al archivo*/
    x = 1;
    y = 0;

    /* Leemos y mostramos el archivo */
    strcpy(direccionArchivo, direccion);
    lectura ();
    imprimeTexto();

    int flag = 1;
    while(flag){
        move(posY, posX);

        noecho();
        keypad(stdscr, true);
        int key = getch();
        //printw(" l%dl", key);

        if (key == KEY_LEFT){
            mover(-1, 0);
            comprobarScroll(-1, 0);
        }
        else if (key == KEY_RIGHT){
            mover(1, 0);
        }
        move(posY, posX);
        imprimeTexto();
        refresh();    


    }
    getch();
    endwin();
    return 0;
}

int main(){
    abrirArchivo("lolo/Ejemplo2.txt");   
    return 0; 
}