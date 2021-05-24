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
    mostrarHeaderFooter();
    move(2, 1);

    for(int espY = 2; espY<= maxY-5; espY++){ // No se debe desbordar horizontalmente
        /* Obtenemos una copia*/
        char *aux = data[y + (espY-2)];

        // Para dejar el puntero en la posicion correcta en x (para scroll)
        for(int i=0; i <x; i++){
            aux++;
        }

        for(int espX = 1; espX <= maxX-2; espX++){ // No se debe desbordar verticalmente

            if(*aux != 00){ // Si no está vacía la cadena
                move(espY, espX);
                printw("%c", *aux);
                aux++;
            }    
        }
        
    }
}
void insertarCaracter(char caracter){

}

int abrirArchivo (char *direccion){
    /* Creamos ventana y sus elementos*/
    crearVentana();
    mostrarHeaderFooter();
    /* Donde partira a imprimir respecto a la ventana*/
    posX = 1;
    posY = 2;
    /* Donde partira a imprimir respecto al archivo*/
    x = 0;
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
        move(10, 10);

        if (key == KEY_LEFT){ // Flecha izquierda
            mover(-1, 0);
            comprobarScroll(-1, 0);
        }
        else if (key == KEY_RIGHT){ // Flecha derecha
            mover(1, 0);
            comprobarScroll(1, 0);
        }
        else if (key == KEY_UP){ // Flecha arriba       
            comprobarScroll(0, -1);
        }
        else if (key == KEY_DOWN){ // Flecha abajo
            mover(0, 1);
            comprobarScroll(0, 1);
        }
        else if (key == 262){ // Tecla inicio
            x = 0;
            y = 0;
            posX = 1;
            posY = 2;
        }
        else if (key == ' '){ // Tecla espacio
            insertarCaracter();
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