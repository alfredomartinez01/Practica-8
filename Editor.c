/* Interfaz editor*/
#include "FuncEditor.c"

void finalizar(int snum){
		exit(0);
}

int abrirArchivo (char *direccion){
    crearVentana();
    mostrarHeaderFooter();
    posX = 5;
    posY = 5;
    /*Compartiendo las coordeanadas del mono entre los procesos*/
	int *xM = shmat(shmget(ftok("Prac8",'k'), sizeof(int),IPC_CREAT|0600), 0, 0);
	int *yM = shmat(shmget(ftok("Prac8",'k'), sizeof(int),IPC_CREAT|0600), 0, 0);

    pid_t pid;
	pid = fork();
	if(pid == 0){ // Proceso encargado de dibujar 
		*xM = posX;
		*yM = posY;
		while (1){
			posX = *xM;	
            //posY = *yM;	
		}		
	exit(0);	
	}
	
	else{ // Proceso encargado de detectar movimiento del cursor y el tipeado
		while(1){
            
			//noecho();        
    		cbreak();
    		signal(SIGTERM, finalizar);
			int key = getch();
			
			if(key == IZQUIERDA){	//Si presiona la tecla "a"
				posX = *xM;
				mover(-2);
			}
			else if(key == DERECHA){	//Si presiona la tecla "d"
				posX = *xM;
				mover(2);
			}
			else{	//Si no se mueve a la derecha o izquierda imprime lo siguiente, esto se puede cambiar para que no imprima nada o no se mueva
					//Lo hice para ver que el movimiento se haga correctamente
				printw(".");
			}
            //printw("%d + %d", posX, posY);
			*xM = posX;

		}			
	}
    return 0;
}
int main(){
    abrirArchivo("sd");   

    getch();         
    endwin();         
    return 0; 
}