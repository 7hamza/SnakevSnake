/**
* \file struct.c
* \brief bibliothèque permet de manipuler deux
* 			structures majeur pour la réalisation du jeux
* \author Jadi Othmane Anwar Hamza
* \version 1.4
* \date Sun 14 Feb 2016
* \date Tue 16 Feb 2016
* \date Wed 17 Feb 2016
* \date Mon 29 Feb 2016
*/
#include "../INCLUDE/snakelib.h"
/**
*	\fn board board_init(int size)
*	\brief initialiser un type de donnée plateau
*		qu'est composé d'un tableau carré d'entier et sa taille
*	\param la taille du plateau
*	\return un plateau initialiser
*/
board
board_init(int x, int y)
{
	board brdout;
	
	brdout.x = x;
	brdout.y = y;
	
	int i, j;
	
	brdout.array=malloc(y*sizeof(char*));
	for(i = 0; i < y; i++)
		brdout.array[i] = malloc(x*sizeof(int));
		

	/*! borders values is '#'. */
	for(i = 0; i < x; i++)
	{
		brdout.array[0][i] = '#';
		brdout.array[y-1][i] = '#';
	}
	for(i = 0;i < y; i++)
	{
		brdout.array[i][0] = '#';
		brdout.array[i][x-1] = '#';
	}
	
	for(i = 1; i < y-1; i++)
		for(j = 1; j < x-1; j++)
			brdout.array[i][j] = ' ';
			
	return brdout;
}
/**
* \fn void board_set(board *brdin,int x,int y,int value)
* \brief pour modifier un plateau (adaptation du système 
* de coordonnée utilisé par la plupart des bibliothèques graphique)
* \param adresse d'un plateau pour accéder et modifier
* simplement ses variables
* \param x et y la case à modifier
* \param value: la nouvelle valeur
* \return une fonction de type void
*/
void
board_set(board *brdin,int x,int y,char value)
{
	brdin->array[y][x] = value;
}
char
board_get(board const* brdin, int x, int y)
{
	return brdin->array[y][x];
}
int
board_size_x(board const* b)
{
	return b->x;
}
int
board_size_y(board const* b)
{
	return b->y;
}
/**
* \fn void board_free(board *brdin)
* \brief liberer la memoire occupé par un plateau
* \param adresse du plateau à libérer
* \return une fonction de type void
*/
void
board_free(board *brdin)
{
	int i;
	
	for(i = 0; i < brdin->y ; i++)
	{
		free(brdin->array[i]);
		brdin->array[i] = NULL;
	}
	
	free(brdin->array);
	brdin->array = NULL;
}
/**
* \fn void board_print(const board *brdin)
* \brief pour afficher un plateau
* \param une adresse d'un plateau concéder constant
*			(droit que pour la lecture) pour éviter de
*			copier le plateau
* \return une fonction de type void
*/
void
board_print(const board *brdin)
{
	int i,j;
	/* traversing 2d-array */
	for(i=0 ; i < brdin->y; i++)
	{
		for(j=0 ;j < brdin->x; j++)
		{
			printf("%c", brdin->array[i][j]);
		}
		printf("\n");
	}
}
/**
*	\fn void board_pxmap(const board *brdin,
*		char* foldername,int filename,int zoom)
*	\brief créer une image ppm du tableau pour mieux
*		voir et interpreter nos fonctions de teste
*	\param une adresse d'un plateau concéder constant
*		(droit que pour la lecture) pour éviter de copier le plateau
*	\param char* foldername: donner un nom au dossier créé
*	\param int filename: numero de la map
*	\param int zoom: pour definir le zoom appliqué a la map
*		( taille d'une case en px = zoom * 1px )
*	\return une fonction de type void
*/
void
board_pxmap(const board *brdin, char* foldername,\
				 int filename, int zoom)
{
	struct stat dir_stat;
	if(stat(foldername, &dir_stat) < 0)
		/*! le dossier n'existe pas */
		mkdir(foldername, S_IRWXU);
	
	
	char ffilename[strlen(foldername) + 7];
	sprintf(ffilename, "%s/%d.ppm", foldername, filename);
	FILE* fdout = fopen( ffilename, "w");
	fprintf(fdout, "P3\n%d %d\n255\n", brdin->y * zoom,\
			brdin->x * zoom);
			
	
	int i,j,l,k;
	/* traversing 2d-array */
	for(i = 0; i < brdin->y; i++)
	{
	 for(l = 0; l < zoom; l++)
	 {
	  for(j = 0; j < brdin->x; j++)
	  {
	   for(k = 0; k < zoom; k++)
	   {
		 if(brdin->array[i][j] == ' ')
		  fprintf(fdout,"175 175 175 ");
		 else if(brdin->array[i][j] == '#')
		  fprintf(fdout,"48 48 48 ");
		 else if(brdin->array[i][j] == '@')
		  fprintf(fdout,"110 11 20 ");
	   }
	  }
	  fprintf(fdout, "\n");
	 }
	 fprintf(fdout, "\n");
	}
	fclose(fdout);
	
	
}
/**
*	\fn void snake_add(snake *snkin,int x,int y)
*	\brief un snake est équivalent à une file d'attente
*		(FIFO « first in, first out »), cette fonction
*		permet d'ajouter à l'entête de la liste
*	\param snake *snkin: adresse d'un snake
*	\param int x, int y: coordonnée du nouvel élément
*	\return fonction de type void
*/
void
snake_add(snake *snkin, int x, int y)
{
	/*! define list output  with coord x,y */
	snake snkout=malloc(sizeof(_snake));
	snkout->x = x;
	snkout->y = y;
	/*! the next element is list input */
	snkout->next = *snkin;
	/*! edit list input( *input=output) */
	*snkin = snkout;
}
/**
*	\fn void snake_del(snake *snkin,int *x,int *y)
*	\brief un snake est équivalent à une file d'attente
*		(FIFO « first in, first out »), cette fonction
*		permet de supprimer au queue de la liste
*	\param snake *snkin: adresse d'un snake
*	\param snake int *x, int *y: adresses de deux
*		entiers pour pouvoir récupérer les coordonnées
*		d'élément supprimer
*	\return fonction de type void
*/
void
snake_del(snake *snkin, int* x, int* y)
{
	if(*snkin == NULL)
	{
		/*! if the list is empty */
		if((x != NULL) && (y != NULL))
		{
			/*! default value x=-1 y=-1 */
			*x = -1;
			*y = -1;
		}
	}
	else if((*snkin)->next == NULL)
	{

		/*! if the list contains only one item */
		if((x != NULL) && (y != NULL))
		{
			*x = (*snkin)->x;
			*y = (*snkin)->y;
		}
		free((*snkin));
		*snkin = NULL;
	}
	else
	{
		/**
		* we move along the linked list
		* keeping the last two consecutive element
		*/
		snake tmp1 = *snkin;
		snake tmp2 = *snkin;
		while(tmp1->next != NULL)
		{
			tmp2 = tmp1;
			tmp1 = tmp1->next;
		}
		if((x != NULL) && (y != NULL))
		{
			*x = tmp1->x;
			*y = tmp1->y;
		}
		tmp2->next = NULL;
		free(tmp1);
	}
}
void
snake_addl(snake *snkin, int x, int y)
{
	snake new = malloc(sizeof(_snake));
	*new = (_snake)
	{
		.x = x,
		.y = y,
		.next = NULL,
	};
	snake tmp = *snkin;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	tmp->next = new;
}
void
snake_getl(snake *snkin, int* x, int* y)
{
	if(*snkin == NULL)
	{
		*x = -1;
		*y = -1;
	}
	else if((*snkin)->next == NULL)
	{
		*x = (*snkin)->x;
		*y = (*snkin)->y;
	}
	else
	{
		snake tmp = *snkin;
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		*x = tmp->x;
		*y = tmp->y;
	}
}
/**
* \fn void snake_free(snake *snkin)
* \brief libérer la mémoire 
* \param adresse du snake
* \return fonction de type void
*/
void
snake_free(snake *snkin)
{
	snake tmp = *snkin;
	snake tmpnext;
	
	while(tmp != NULL)
	{
		tmpnext = tmp->next;
		free(tmp);
		tmp = tmpnext;
	}
	
	*snkin = NULL;
}
/**
*	\fn void snake_print(snake snkin)
*	\brief afficher les éléments d'un snake
*	\param const snake *snkin: adresse d'un snake
*		concéder constant (droit que pour la lecture)
*		pour éviter de copier les éléments du snake
*	\return fonction de type void
*/
void
snake_print(const snake *snkin)
{
	snake tmp = *snkin;
	while(tmp != NULL)
	{
		printf("[%d:%d]", tmp->x, tmp->y);
		tmp = tmp->next;
	}
	printf("\n");
}


int
snake_x(snake const *s)
{
	return (*s)->x;
}
int
snake_y(snake const *s)
{
	return (*s)->y;
}
snake*
snake_next(snake const* s)
{
	return &((*s)->next);
}
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
void
move(board *brdin,snake *snkin, char id)
{
	int x,y,dx,dy;
	snake_del(snkin, &x, &y);
	board_set(brdin, x, y, ' ');
	dx = ((*snkin)->x) - ((*snkin)->next->x);
	dy = ((*snkin)->y) - ((*snkin)->next->y);
	snake_add(snkin, (*snkin)->x+dx , (*snkin)->y+dy);
	board_set(brdin, (*snkin)->x ,(*snkin)->y , id);
}
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
void
turn(board *brdin,snake *snkin,int drctn,char id)
{
	int x,y,dx,dy;
	
	snake_del(snkin,&x,&y);
	board_set(brdin,x,y, ' ');
	
	dx=((*snkin)->x) - ((*snkin)->next->x);
	dy=((*snkin)->y) - ((*snkin)->next->y);
	
	if(dx == 1)
	{
		snake_add(snkin, (*snkin)->x, (*snkin)->y + drctn);
		board_set(brdin, (*snkin)->x, (*snkin)->y, id);
	}
	
	else if(dx == -1)
	{
		snake_add(snkin, (*snkin)->x, (*snkin)->y - drctn);
		board_set(brdin, (*snkin)->x, (*snkin)->y, id);
	}
	
	else if(dy == 1)
	{
		snake_add(snkin,(*snkin)->x-drctn,(*snkin)->y);
		board_set(brdin,(*snkin)->x,(*snkin)->y,id);
	}
	
	else if(dy == -1)
	{
		snake_add(snkin,(*snkin)->x+drctn,(*snkin)->y);
		board_set(brdin,(*snkin)->x,(*snkin)->y,id);
	}
}
void
snake_init(board *brdin,snake *snkin, int len, char id)
{
	if(id == '@')
	{
		
		snake_add( snkin, brdin->x / 2, brdin->y - 2 );
		board_set( brdin, (*snkin)->x, (*snkin)->y, id );
		
		int i;
		for(i = 0; i < len - 1; i++)
		{
			snake_add( snkin, (*snkin)->x, (*snkin)->y -1 );
			board_set( brdin, (*snkin)->x, (*snkin)->y, id );
		}
		
	}
	
	if(id == '&')
	{
		
		snake_add( snkin, brdin->x / 2, 1 );
		board_set( brdin, (*snkin)->x, (*snkin)->y, id );
		
		int i;
		for(i = 0; i < len - 1; i++)
		{
			snake_add( snkin, (*snkin)->x, (*snkin)->y +1 );
			board_set( brdin, (*snkin)->x, (*snkin)->y, id );
		}
		
	}

	if(id == '+')
	{
		
		snake_add( snkin, 1, brdin->y / 2 );
		board_set( brdin, (*snkin)->x, (*snkin)->y, id );
		
		int i;
		for(i = 0; i < len - 1; i++)
		{
			snake_add( snkin, (*snkin)->x + 1, (*snkin)->y);
			board_set( brdin, (*snkin)->x, (*snkin)->y, 1 );
		}
		
	}
	
}
bool
choc_snake(snake const* s1, snake const* s2)
{
	snake tmp = (*s2)->next;
	while(tmp)
	{
		if((*s1)->x == tmp->x && (*s1)->y == tmp->y)
			return true;
		tmp = tmp->next;
	}
	return false;
}
bool
choc_sc(snake const* s1, snake const* s2)
{
	if ((*s1)->x == (*s2)->x && (*s1)->y == (*s2)->y)
		return true;
	return false;
}
bool
choc_wall(board const* b, snake const* s)
{
	if((*s)->x == 0) return true;
	if((*s)->x == b->x-1) return true;
	if((*s)->y == 0) return true;
	if((*s)->y == b->y-1) return true;
	
	return false;
}
int
rand_a_b(int a, int b)
{
	return (rand()%(b-a) + a);
}
void
board_apple(board* b, int *x, int *y)
{
	bool t = true;
	while(t)
	{
			*x = rand_a_b(1, b->x-2);
			*y = rand_a_b(1, b->y-2);
			if(board_get(b,*x,*y) == ' ')
			{
				board_set(b,*x,*y,'$');
				t = false;
			}
	}
}
bool
snake_apple(snake const* s, int *x, int *y)
{
	if(snake_x(s) == *x && snake_y(s) == *y) return true;
	return false;
}
