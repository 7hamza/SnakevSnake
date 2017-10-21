#include "../INCLUDE/snakelib.h"
#include "../INCLUDE/schlangalib.h"
#include <SDL/SDL.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h> /* pour time */

#define BOARD_SIZE_X 68
#define BOARD_SIZE_Y 33
#define SNAKE_LEN 12
#define SLEEP 105000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool X;
SDL_Surface *E, *A, *B, *C, *D;
board b;
snake s;
snake c;

void *sdl_turn();
void *sdl_move();
void sdl_print();
int main()

{
	/* rand init */
	srand(time(0));
	X = true;
	/* init board + snake */
	b = board_init(BOARD_SIZE_X,BOARD_SIZE_Y);
	snake_init(&b,&s,SNAKE_LEN,'@');
	snake_init(&b,&c,SNAKE_LEN,'&');
	/* init sdl en mode video */
	SDL_Init(SDL_INIT_VIDEO);
	/* init surface E avec une taille 20*BOARD_SIZE_X et 20*BOARD_SIZE_Y */
	E = SDL_SetVideoMode(20*BOARD_SIZE_X, 20*BOARD_SIZE_Y, 32, SDL_HWSURFACE);
	/* creer le reste des surfaces avec une taille de 20*20 px */
	A = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, 32, 0, 0, 0, 0);
	B = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, 32, 0, 0, 0, 0);
	C = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, 32, 0, 0, 0, 0);
	D = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, 32, 0, 0, 0, 0);
	/* init avec couleur rgb */
	SDL_FillRect(A, 0, SDL_MapRGB(E->format, 95, 0, 0));
	SDL_FillRect(B, 0, SDL_MapRGB(E->format, 137, 137, 137));
	SDL_FillRect(C, 0, SDL_MapRGB(E->format, 47, 47, 47));
	SDL_FillRect(D, 0, SDL_MapRGB(E->format, 157, 62, 12));
	/* init barre de fenetre */
	SDL_WM_SetCaption("Snack", 0);
	/* nos variabe de thread */
	pthread_t thread_turn;
	pthread_t thread_move;
	/* creer les thread */
	pthread_create(&thread_turn, 0, &sdl_turn, 0);
	pthread_create(&thread_move, 0, &sdl_move,0);
	/* attendre leurs fin */
	pthread_join(thread_turn, 0);
	pthread_join(thread_move, 0);
	/* libere surface */
	SDL_FreeSurface(E);
	SDL_FreeSurface(A);
	SDL_FreeSurface(B);
	SDL_FreeSurface(C);
	/* libere board + snake */
	board_free(&b);
	snake_free(&s);
	snake_free(&c);
	SDL_Quit();
	return 0;
}








void *sdl_turn() 
{
	SDL_Event event;
	while (X) 
	{
	SDL_WaitEvent(&event);
		switch (event.type) 
		{
			case SDL_QUIT:
				X = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						X = false;
						break;
					case SDLK_RIGHT:/* key right */
					case SDLK_KP3:/* keypad 3 */
					case SDLK_KP9:/* keypad 3 */
					case 'e':/* code ascii */
						/* on ferme la variable mutex */
						pthread_mutex_lock(&mutex);
						/* turn snake */
						turn(&b,&s,1,'@');
						schlanga_ia(&b,&s,&c,SNAKE_LEN);
						if ((X = !(choc_snake(&s,&s))\
							&& !(choc_wall(&b,&s))\
							&& !(choc_snake(&c,&c))\
							&& !(choc_wall(&b,&c))\
							&& !(choc_snake(&c,&s))\
							&& !(choc_snake(&s,&c))))
						
						{
							sdl_print();
						}
						pthread_mutex_unlock(&mutex);
						break;
					case SDLK_LEFT:/* key left */
					case SDLK_KP1:/* keypad 1*/
					case SDLK_KP7:/* keypad 7*/
					case 'a':/* code ascii */
						/* on ferme la variable mutex */
						pthread_mutex_lock(&mutex);
						/* turn snake */
						turn(&b,&s,-1,'@');
						schlanga_ia(&b,&s,&c,SNAKE_LEN);
						if ((X = !(choc_snake(&s,&s))\
							&& !(choc_wall(&b,&s))\
							&& !(choc_snake(&c,&c))\
							&& !(choc_wall(&b,&c))\
							&& !(choc_snake(&c,&s))\
							&& !(choc_snake(&s,&c))))
						{
							sdl_print();
						}
						pthread_mutex_unlock(&mutex);
						break;
					default:/* ne fait rien */break;
				}	
			default:/* ne fait rien */break;
		}
	}
	pthread_exit(0);
}
void *sdl_move() 
{
	while(X) 
	{
			/* on ferme la variable mutex */
			pthread_mutex_lock(&mutex);
			/* move snake */
			move(&b,&s,'@');
			schlanga_ia(&b,&s,&c,SNAKE_LEN);
			if ((X = !(choc_snake(&s,&s))\
				&& !(choc_wall(&b,&s))\
				&& !(choc_snake(&c,&c))\
				&& !(choc_wall(&b,&c))\
				&& !(choc_snake(&c,&s))\
				&& !(choc_snake(&s,&c))))
			
			{
				sdl_print();
			}
			/* libere le mutex pour les autres thread */
			pthread_mutex_unlock(&mutex);
			usleep(SLEEP);
		}
		pthread_exit(0);
}



void sdl_print() 
{
	int x,y, dx = board_size_x (&b), dy = board_size_y (&b);
	/* def une position */
	SDL_Rect p;
	for (x = 0; x < dx; x++) 
	{
		for (y = 0; y < dy; y++) 
		{
			p.x = x * 20 ; p.y = y * 20 ;
			/* lire dans le plateau et dessiner la surface */
			switch (board_get (&b,x,y)) 
			{
				case '@':
					SDL_BlitSurface(A, 0, E, &p);
					break;
				case ' ':
					SDL_BlitSurface(B, 0, E, &p);
					break;
				case '#':
					SDL_BlitSurface(C, 0, E, &p);
					break;
				case '&':
					SDL_BlitSurface(D, 0, E, &p);
					break;
				}
		}
	}
	board_print(&b);
	/* reafficher un nouvelle ecran */
	SDL_Flip(E);
}