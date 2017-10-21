#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "../INCLUDE/snakelib.h"

#include <SDL/SDL.h>

#define err(msg) {fprintf(stderr, "%s\n", msg);exit(1);}
#define PORT 1234
#define ADDR "localhost"

#define tabX 51
#define tabY 31
#define slen 3

pthread_t threadpt;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

SDL_Surface *E, *A, *B, *C, *D;

board b;
snake s, c;

bool X;

char t, o, r, l, m;
int n;
static int client_socket()
{
	int n = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > n) err("socket()");

	struct hostent* server_host = gethostbyname(ADDR);
	if(NULL == server_host) err("gethostbyname()");

	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	bcopy((char *)server_host->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, server_host->h_length);
	if(0 > connect(n, (struct sockaddr*)&server_addr, sizeof(server_addr))) err("connect()");
	return n;
}


void sdl_print() {
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
	/* reafficher un nouvelle ecran */
	SDL_Flip(E);
}



void* sdl_move()
{
	char buff;
	while(X)
	{
		/* on ferme la variable mutex */
		pthread_mutex_lock(&mutex);
		/* move snake */
		move(&b,&s,t);
		write(n,&m,1);
		read(n,&buff,sizeof(char*));
		switch(buff)
		{
			case 'm':
				move(&b,&c,o);
				break;
			case 'l':
				turn(&b,&c,o,-1);
				break;
			case 'r':
				turn(&b,&c,o,1);
				break;
		}
		/* affiche SDL */
		if ((X = !(choc_snake(&s,&s)) && !(choc_wall(&b,&s))))
			sdl_print();
		/* libere le mutex pour les autres thread */
		pthread_mutex_unlock(&mutex);
		/*u*/sleep (1/*00000*/);
	}
	pthread_exit(0);
}

void* sdl_turn()
{
	SDL_Event event;
	char buff;
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
						turn(&b,&s,1,t);
						write(n,&r,1);
						read(n,&buff,sizeof(char*));
						switch(buff)
						{
							case 'm':
								move(&b,&c,o);
								break;
							case 'l':
								turn(&b,&c,o,-1);
								break;
							case 'r':
								turn(&b,&c,o,1);
								break;
						}
						/* affiche SDL */
						if ((X = !(choc_snake(&s,&s)) && !(choc_wall(&b,&s))))
							sdl_print();
						pthread_mutex_unlock(&mutex);
						break;
					case SDLK_LEFT:/* key left */
					case SDLK_KP1:/* keypad 1*/
					case SDLK_KP7:/* keypad 7*/
					case 'a':/* code ascii */
						/* on ferme la variable mutex */
						pthread_mutex_lock(&mutex);
						/* turn snake */
						turn(&b,&s,-1,t);
						write(n,&l,1);
						read(n,&buff,sizeof(char*));
						switch(buff)
						{
							case 'm':
								move(&b,&c,o);
								break;
							case 'l':
								turn(&b,&c,o,-1);
								break;
							case 'r':
								turn(&b,&c,o,1);
								break;
						}
						/* affiche SDL */
						if ((X = !(choc_snake(&s,&s)) && !(choc_wall(&b,&s))))
							sdl_print();
						/* libere le mutex pour les autres thread */
						pthread_mutex_unlock(&mutex);
						break;
					default:/* ne fait rien */break;
				}	
			default:/* ne fait rien */break;
		}
	}
	pthread_exit(0);
}

int main()
{
	n = client_socket();
	read(n,&t,sizeof(char*));
	o = (t == '@')? '&' : '@';
	m = 'm';
	r = 'r';
	l = 'l';
	/* la variable globale du jeu */
	X = true;
	/* rand init */
	srand(time(0));
	/* init board + snake */
	b = board_init(tabX,tabY);
	snake_init(&b,&s,slen,t);
	snake_init(&b,&c,slen,o);
	/* init sdl en mode video */
	SDL_Init(SDL_INIT_VIDEO);
	/* init surface E avec une taille 20*tabX et 20*tabY */
	E = SDL_SetVideoMode(20*tabX, 20*tabY, 32, SDL_HWSURFACE);
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
	pthread_create (&thread_turn, 0, &sdl_turn, 0);
	pthread_create (&thread_move, 0, &sdl_move,0);
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