/* Menu inicial */

#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h> 
#include <dirent.h>
#include "Editor.c"



#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
#define NOM_MAX 80

void crearCarpeta();
void crearArchivo();
void verArchivos();
void getNombreArchivo();
void eliminarArchivo();
void moverse();

void footerMenu();
void volverMenu();

char *choices[] = {
                       "Crear Carpeta",
                       "Crear Archivo",
                       "Abrir Archivo",
                       "Eliminar Archivo",
			"Moverse",
                  };

int main()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
        int n_choices, i;
	ITEM *cur_item;
	WINDOW *menu;  
	
	/* Initialize curses */	
	menu = initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(2, COLOR_BLUE, COLOR_WHITE);
	init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
	bkgd(COLOR_PAIR(1));

	/* Initialize items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], "");
	

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set menu mark to the string " ~ " */
        set_menu_mark(my_menu, " ~ ");

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE); //SUBRAYADO
	set_menu_back(my_menu, COLOR_PAIR(2)); //TEXTO
	set_menu_grey(my_menu, COLOR_PAIR(3)); //FONDO

	/* Post the menu */
	footerMenu();
	post_menu(my_menu);
	refresh();

	while((c = getch()) != KEY_F(1)){       
		switch(c){
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
				
			case 10: /* Enter */
				move(20, 0);
				clrtoeol();
				
				if(strcmp(item_name(current_item(my_menu)), "Crear Carpeta")==0){
					clear();
					crearCarpeta();
					volverMenu();
					menu_driver(my_menu, REQ_UP_ITEM);
					menu_driver(my_menu, REQ_DOWN_ITEM);
				}
					
				else if(strcmp(item_name(current_item(my_menu)), "Crear Archivo")==0){
					clear();
					crearArchivo();
					volverMenu();
					menu_driver(my_menu, REQ_UP_ITEM);
					menu_driver(my_menu, REQ_DOWN_ITEM);
				}
				else if(strcmp(item_name(current_item(my_menu)), "Abrir Archivo")==0){
					clear();
					getNombreArchivo();
					
					init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
					init_pair(2, COLOR_BLUE, COLOR_WHITE);
					init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
					bkgd(COLOR_PAIR(1));
					set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE); //SUBRAYADO
					set_menu_back(my_menu, COLOR_PAIR(2)); //TEXTO
					set_menu_grey(my_menu, COLOR_PAIR(3)); //FONDO
					
					volverMenu();
					menu_driver(my_menu, REQ_UP_ITEM);
					menu_driver(my_menu, REQ_DOWN_ITEM);
				}	
				
				else if(strcmp(item_name(current_item(my_menu)), "Eliminar Archivo")==0){
					clear();
					eliminarArchivo();
					volverMenu();
					menu_driver(my_menu, REQ_UP_ITEM);
					menu_driver(my_menu, REQ_DOWN_ITEM);
				}
				
				else if(strcmp(item_name(current_item(my_menu)), "Moverse")==0){
					clear();
					moverse();
					volverMenu();
					menu_driver(my_menu, REQ_UP_ITEM);
					menu_driver(my_menu, REQ_DOWN_ITEM);
				}
				
				pos_menu_cursor(my_menu);
				break;
		}
	}
	unpost_menu(my_menu);
	for(i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}

void footerMenu(){
	char cwd[PATH_MAX]; //Constante que se encuentra en limits.h 
	move(LINES-5, 0);
	printw("Direccion: < %s >\n", getcwd(cwd, sizeof(cwd))); //Mostrar direccion
	for(int i = 0; i<COLS; i++){
		move(LINES-4, i);
		printw("-");
    	}
	move(LINES-3, 0);
	printw("Presiona <ENTER> en la opcion deseada");
	move(LINES-2, 0);
	printw("Navegar con las teclas de flechas de arriba y abajo (F1 para salir)");
}

void volverMenu(){
	printw("\nVolviendo al menú...");
	refresh();
	sleep(3);
	clear();
	footerMenu();
}


void crearCarpeta(){
	int check;
    	char *nombre = (char*)malloc(sizeof(char)*NOM_MAX);
  	echo();
  	move(LINES, 0);
  	printw("Escribe el nombre del directorio: ");
	getnstr(nombre, NOM_MAX);
  	
    	check = mkdir(nombre,0777);
  

	if (!check)
		printw("Directorio creado con exito\n");
	else {
		printw("Ya existe un directorio con ese nombre\n");
	}
}

void crearArchivo(){
	char *nombre = (char*)malloc(sizeof(char)*NOM_MAX);
	echo();
	printw("Escribe el nombre del archivo: ");
	getnstr(nombre, NOM_MAX);
	
	FILE *archivo;
	char *nom = strcat(nombre, ".txt");
	if(fopen(nom, "r") == NULL){
		archivo = fopen(nom,"w");
		fclose(archivo);
		printw("Archivo creado con exito\n");
	}
	else
		printw("Ya existe un archivo con ese nombre\n");	
}

void verArchivos(){
	/* Con un puntero a DIR abriremos el directorio */
	DIR *dir;
	/* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
	struct dirent *ent;

	/* Empezaremos a leer en el directorio actual */
	dir = opendir (".");

	/* Miramos que no haya error */
	if (dir == NULL){
		printw("No pudo abrirse el directorio\n");
  		exit(EXIT_FAILURE);
	}
	else{
		/* Leyendo uno a uno todos los archivos que hay */
		while ((ent = readdir (dir)) != NULL){
			/* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
			if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
				/* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
				//procesoArchivo(ent->d_name);
				FILE *fich=fopen(ent->d_name, "r");
				if (fich){
					fseek(fich, 0L, SEEK_END);
					fclose(fich);
					/* Si todo va bien, decimos el tamaño */
					printw ("%30s \n", ent->d_name);
				}
			}
		}
		closedir (dir);
	}
}

void getNombreArchivo(){
	char *nombre = (char*)malloc(sizeof(char)*NOM_MAX);
	echo();
	verArchivos();
	printw("\n\nEscribe el nombre del archivo: ");
	getnstr(nombre, NOM_MAX);
	char *nom = strcat(nombre, ".txt");
	if(fopen(nom, "r") == NULL){
		printw("Archivo no encontrado\n");
	}
	else{
		abrirArchivo(nom);
	}
	
}

void eliminarArchivo(){
	char *nombre = (char*)malloc(sizeof(char)*NOM_MAX);
	echo();
	verArchivos();
	printw("\n\nEscribe el nombre del archivo: ");
	getnstr(nombre, NOM_MAX);
	
	FILE *archivo;
	char *nom = strcat(nombre, ".txt");
	if(fopen(nom, "r") == NULL){
		printw("Archivo no encontrado\n");
	}
	else{
		int del = remove(nom);
		if (!del)
			printw("Archivo << %s >> eliminado exitosamente\n", nombre);
		else
			printw("Error, no se pudo eliminar el archivo\n");
	}
	
}

void moverse(){
	char *direccion = (char*)malloc(sizeof(char)*PATH_MAX);
	char cwd[PATH_MAX];
	echo();
	printw("Usted, está en la ruta: %s\n\n", getcwd(cwd, sizeof(cwd)));
	printw("Escribe la ruta a la que quiere moverse: ");
	getnstr(direccion, PATH_MAX);
	if(chdir(direccion) == 0)
		printw("Se ha movido de ruta correctamente\n");
	else
		printw("Error, ruta no encontrada\n");
}
