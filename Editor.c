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

/* Funcion dedicada a la escritura del archivo*/
void escritura(){
    FILE *archivo = fopen(direccionArchivo, "w");
    for(int i = 0; i < lineasArchivo; i++){
        fputs(data[lineasArchivo], archivo);
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
        //move(10, 10);
        //printw("%d", key);

        /* NOTA:(posX-1)+x  es la posicion sobre el cadena de caracteres respecto al cursor en x
                (posY-2)+y  es la posicion sobre la cadena de caracteres respecto al cursor en y*/

        if (key == KEY_LEFT){ // Flecha izquierda
            if((posX-1)+x -1 <= tamanoLinea((posY-2)+y)){ // Comprueba que el tamaño de linea permita moverlo
                mover(-1, 0);
                comprobarScroll(-1, 0);
            }
        }
        else if (key == KEY_RIGHT){ // Flecha derecha
            if( ((posX-1)+x) + 1 <= tamanoLinea((posY-2)+y)){ // Comprueba que el tamaño de linea permita moverlo
                mover(1, 0);
                comprobarScroll(1, 0);
            }
        }
        else if (key == KEY_UP){ // Flecha arriba   
            mover(0, -1);    
            comprobarScroll(0, -1);
            if(tamanoLinea((posY-2)+y) < tamanoLinea((posY-2)+y+1) && posX+1 > tamanoLinea((posY-2)+y)){ // Condiciones para saber donde se coloca el cursor, puede hacer retorno de carro en la siguiente linea
                    x = 0;
                    posX = 1;
                    while( ((posX-1)+x) + 1 <= tamanoLinea((posY-2)+y)){ // Se va al final de la linea
                        mover(1, 0);
                        comprobarScroll(1, 0);
                    }
            }
        }
        else if (key == KEY_DOWN){ // Flecha abajo
            if( (posY-2)+y + 1 < lineasArchivo-1){  // Comprueba que la linea siguiente sí sea una linea existente              
                mover(0, 1);
                comprobarScroll(0, 1);

                if(tamanoLinea((posY-2)+y) < tamanoLinea((posY-2)+y-1) && posX-1 > tamanoLinea((posY-2)+y)){ // Condiciones para saber donde se coloca el cursor, puede hacer retorno de carro en la siguiente linea
                    x = 0;
                    posX = 1;
                    while( ((posX-1)+x) + 1 <= tamanoLinea((posY-2)+y)){ // Se va al final de la linea
                        mover(1, 0);
                        comprobarScroll(1, 0);
                    }
                }
            }
        }
        else if (key == 262){ // Tecla inicio (retorno de carro)
            x = 0;
            y = 0;
            posX = 1;
            posY = 2;
        }
        else if (key == KEY_END){ // Tecla fin (se va al final)
            while( ((posX-1)+x) + 1 <= tamanoLinea((posY-2)+y)){
                mover(1, 0);
                comprobarScroll(1, 0);
            }
        }
        else if (key == 263){ // Algun caracter a eliminar
            if(tamanoLinea((posY-2)+y) > 0){ // comprueba que sí haya caracteres que eliminar, para evitar conflictos con el cursor
                eliminarCaracter();
            }
        }
        else if (key >= 32 && key<= 255){ // Algun caracter a insertar
            insertarCaracter(key);
        }
        else if(key = 27){ // Escritura de las lineas que se tienen en memoria
            // Declaramos el arreglo para la lectura de todo el archivo
            for(int i=0; i < 100; i++){
                data[i] = (char*)malloc(10000*sizeof(char)); 
            }
            direccionArchivo = (char *) malloc(100*sizeof(char));

            escritura();
            lectura();
        }
        imprimeTexto();
        refresh();
    }
    getch();
    endwin();
    return 0;
}

int main(){
    abrirArchivo("ejemplo.txt");   
    return 0; 
}