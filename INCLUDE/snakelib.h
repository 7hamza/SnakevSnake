/**
* \file struct.h
* \brief bibliothèque permet de manipuler deux structures majeur pour la réalisation du jeux
* \author Jadi Othmane Anwar Hamza
* \version 1.2
* \date Tue 16 Feb 2016
*/
#ifndef _STRUCT_H
#define _STRUCT_H
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/**
* \struct board struct.h
* \brief int **array and int size tableau 2d et sa taille
*/
typedef struct board board;
struct board
{
	char **array;
	int x;
	int y;
};
/**
* \struct snake struct.h
* \brief linked list avec deux entiers x et y représente chaque élément du snake
*/
typedef struct _snake _snake;
struct _snake
{
	int x;
	int y;
	struct _snake *next;
};
typedef _snake* snake;
/**
* \fn int board_size_x(board const* b)
* \brief taille x du plateau
* \param adresse du plateau
*/
extern int board_size_x(board const* b);
/**
* \fn int board_size_x(board const* b)
* \brief taille y du plateau
* \param adresse du plateau
*/
extern int board_size_y(board const* b);
/**
* \fn board board_init(int size)
* \brief initialiser un type de donnée plateau qu'est composé d'un tableau carré d'entier et sa taille
* \param la taille du plateau
* \return un plateau initialiser
*/
extern board board_init(int x, int y);
/**
* \fn void board_set(board *brdin,int x,int y,int value)
* \brief pour modifier un plateau (adaptation du système de coordonnée utilisé par la plupart des bibliothèques graphique)
* \param adresse d'un plateau pour accéder et modifier simplement ses variables
* \param x et y la case à modifier
* \param value: la nouvelle valeur
* \return une fonction de type void
*/
extern void board_set(board *brdin,int x,int y,char value);
/**
* \fn char board_get(board const* brdin, int x, int y)
* \brief return la valeur 
* \param adresse d'un plateau pour accéder et modifier simplement ses variables
* \param x et y la case à visiter
* \return char la valueur du point (x,y)
*/
extern char board_get(board const* brdin, int x, int y);
/**
* \fn void board_free(board *brdin)
* \brief liberer la memoire occupé par un plateau
* \param adresse du plateau à libérer
* \return une fonction de type void
*/
extern void board_free(board *brdin);
/**
* \fn void board_print(const board *brdin)
* \brief pour afficher un plateau
* \param une adresse d'un plateau concéder constant (droit que pour la lecture) pour éviter de copier le plateau
* \return une fonction de type void
*/
extern void board_print(const board *brdin);
/**
* \fn void board_pxmap(const board *brdin,char* foldername,int filename,int zoom)
* \brief créer une image ppm du tableau pour mieux voir et interpreter nos fonctions de teste
* \param une adresse d'un plateau concéder constant (droit que pour la lecture) pour éviter de copier le plateau
* \param char* foldername: donner un nom au dossier créé
* \param int filename: numero de la map
* \param int zoom: pour definir le zoom appliqué a la map ( taille d'une case en px = zoom * 1px )
* \return une fonction de type void
*/
extern void board_pxmap(const board *brdin,char* foldername,int filename,int zoom);
/**
* \fn void snake_add(snake *snkin,int x,int y)
* \brief un snake est équivalent à une file d'attente (FIFO « first in, first out »), cette fonction permet d'ajouter à l'entête de la liste
* \param snake *snkin: adresse d'un snake
* \param int x, int y: coordonnée du nouvel élément
* \return fonction de type void
*/
extern void snake_add(snake *snkin,int x,int y);

extern void snake_addl(snake *snkin,int x,int y);
extern void snake_getl(snake *snkin, int* x, int* y);
/**
* \fn void snake_del(snake *snkin,int *x,int *y)
* \brief un snake est équivalent à une file d'attente (FIFO « first in, first out »), cette fonction permet de supprimer au queue de la liste
* \param snake *snkin: adresse d'un snake
* \param snake int *x, int *y: adresses de deux entiers pour pouvoir récupérer les coordonnées d'élément supprimer
* \return fonction de type void
*/
extern void snake_del(snake *snkin,int *x,int *y);
/**
* \fn void snake_free(snake *snkin)
* \brief libérer la mémoire 
* \param adresse du snake
* \return fonction de type void
*/
extern void snake_free(snake *snkin);
/**
* \fn void snake_print(snake snkin)
* \brief afficher les éléments d'un snake
* \param const snake *snkin: adresse d'un snake concéder constant (droit que pour la lecture) pour éviter de copier les éléments du snake
* \return fonction de type void
*/
extern void snake_print(const snake *snkin);
/**
*	\fn void move(board *brdin,snake *snkin,int ref)
*	\brief déplacer le snake
*	\param board *brdin: adresse d'un plateau pour
*		mettre à jour la nouvelle position du snake
*	\param board *snkin: adresse d'un snake pour pouvoir
*		le modifier en ajoutant un nouvel élément a l'entête
*		et supprimer l'élément de la fin de la liste
*	\param int ref: référence du snake
*	\return fonction de type 
*/
extern void move(board *brdin,snake *snkin,char id);
/**
*	\fn void turn(board *brdin,snake *snkin,int drctn,int value)
*	\brief tourner tourner à gauche ou droite left<->right
*	\param board *brdin: adresse d'un plateau pour mettre
*		à jour la nouvelle position du snake
*	\param board *snkin: adresse d'un snake pour pouvoir
*		le modifier en ajoutant un nouvel élément a l'entête
*		et supprimer l'élément de la fin de la liste
*	\param int drctn: direction left=-1 right=1
*	\param int id: id du snake
*	\return fonction de type void
*/
extern void turn(board *brdin,snake *snkin,int drctn,char id);
/**
*	\fn void snake_init(board *brdin,snake *snkin, int len, char id)
*	\brief init a snake
*/
extern void snake_init(board *brdin,snake *snkin, int len, char id);
/**
*	\fn bool choc_snake(snake const* s1, snake const* s2)
*	\brief test choc entre deux snake
*/
extern bool choc_snake(snake const* s1, snake const* s2);
extern bool
choc_sc(snake const* s1, snake const* s2);
/**
*	\fn bool choc_wall(board const* b, snake const* s)
*	\brief test choc snake contre le mur
*/
extern bool choc_wall(board const* b, snake const* s);
extern void board_apple(board* b, int *x, int *y);
extern bool snake_apple(snake const* s, int *x, int *y);
#endif