#include <stdlib.h>
#include <stdbool.h>
#include "../INCLUDE/snakelib.h"
#include "../INCLUDE/schlangalib.h"

#define CODE_NULL 0
#define CODE_BOW 1
#define CODE_WALL 2
#define CODE_CORNER 3
#define CODE_NEXT_TO 4

/*
*	\fn bool exist_in_snake(const snake *s, int x, int y)
* \brief verfier si un point appartient au snake
* \param snake* un snake
*	\param int coordonnée x
* \param int coordonnée y
* \return true s'il appartient au snake, false sinon
*/
bool exist_in_snake(const snake *s, int x, int y)
{
	snake curr = *s;
	while(curr != 0)
	{
		if(curr->x == x && curr->y == y)
		{
			return true;
		}
		curr = curr->next;
	}
	return false;
}

/*
*	\fn schlanga_bow(board* b, const snake* s, snake* c)
* \brief reconnaître la forme géométrique du snake et l'éviter
* \param board* plateau du jeu pour opérer
*	\param snake* snake 
* \param snake* schlanga
* \return true s'il a opéré, false sinon
*/
bool schlanga_bow(board* b, const snake* s, snake* c)
{
	snake curr = *s;
	snake head = *s;
	bool danger = false;
	int schla_dir_x = (*c)->x - (*c)->next->x;
	int schla_dir_y = (*c)->y - (*c)->next->y;
	while(curr != 0)
	{
		if(\
			((*c)->x + schla_dir_x == curr->x) && ((*c)->y == curr->y)\
		)
		{
			danger = true;
			break;
		}
		if(\
			((*c)->x + 2*schla_dir_x == curr->x) && ((*c)->y == curr->y)\
		)
		{
			danger = true;
			break;
		}
		if(\
			((*c)->x + 3*schla_dir_x == curr->x) && ((*c)->y == curr->y)\
		)
		{
			danger = true;
			break;
		}


		if(\
			((*c)->y + schla_dir_y == curr->y) && ((*c)->x == curr->x)\
		)
		{
			danger = true;
			break;
		}
		if(\
			((*c)->y + 2*schla_dir_y == curr->y) && ((*c)->x == curr->x)\
		)
		{
			danger = true;
			break;
		}
		if(\
			((*c)->y + 3*schla_dir_y == curr->y) && ((*c)->x == curr->x)\
		)
		{
			danger = true;
			break;
		}
		curr = curr->next;
	}
	if(danger)
	{
		int snake_dir_x = head->x - head->next->x;
		int snake_dir_y = head->y - head->next->y;
		int snake_bow_x = 0, snake_bow_y = 0;
		int dx = 0, dy = 0;
		curr = *s;
		while(snake_bow_x == 0\
			&& snake_bow_y == 0\
			&& curr->next->next != 0)
		{
			dx = curr->x - curr->next->next->x;
			dy = curr->y - curr->next->next->y;
			if(dx != 0 && dy != 0)
			{
				snake_bow_x = dx;
				snake_bow_y = dy;
				break;
			}
			curr = curr->next;
		}
		if(snake_bow_x == 1\
			&& snake_bow_y == 1)
		{
			if(snake_dir_x == 1)
			{
				turn(b,c,-1,'&');
				return true;
			}
			if(snake_dir_y == 1)
			{
				turn(b,c,1,'&');
				return true;
			}
		}
		if(snake_bow_x == 1\
			&& snake_bow_y == -1)
		{
			if(snake_dir_x == 1)
			{
				turn(b,c,1,'&');
				return true;
			}
			if(snake_dir_y == -1)
			{
				turn(b,c,-1,'&');
				return true;
			}	
		}
		if(snake_bow_x == -1\
			&& snake_bow_y == 1)
		{
			if(snake_dir_x == -1)
			{
				turn(b,c,1,'&');
				return true;
			}
			if(snake_dir_y == 1)
			{
				turn(b,c,-1,'&');
				return true;
			}
		}
		if(snake_bow_x == -1\
			&& snake_bow_y == -1)
		{
			if(snake_dir_x == -1)
			{
				turn(b,c,-1,'&');
				return true;
			}
			if(snake_dir_y == -1)
			{
				turn(b,c,1,'&');
				return true;
			}	
		}
		turn(b,c,(rand() % 2) * 2 - 1,'&');
		return true;
	}
	return false;
}

/*
*	\fn bool schlanga_wall(board* b, snake* c)
* \brief éviter le mur 
* \param board* plateau du jeu pour opérer
* \param snake* schlanga
* \return true s'il a opéré, false sinon
*/
bool schlanga_wall(board* b, snake* c)
{
	int x_board = 1;
	int y_board = 1;
	int dx_board = b->x - 2;
	int dy_board = b->y - 2;
	int x_schla = (*c)->x;
	int y_schla = (*c)->y;
	int dx_schla = (*c)->x - (*c)->next->x;
	int dy_schla = (*c)->y - (*c)->next->y;
	if(y_schla == y_board)
	{
		if(dx_schla == 0)
		{
			if(x_schla == x_board)
			{
				turn(b,c,1,'&');
				return true;
			}
			if(x_schla == dx_board)
			{
				turn(b,c,-1,'&');
				return true;
			}
			turn(b,c,(rand() % 2) * 2 - 1,'&');
			return true;
		}
		turn(b,c,dx_schla,'&');
		return true;
	}
	if(y_schla == dy_board)
	{
		if(dx_schla == 0)
		{
			if(x_schla == x_board)
			{
				turn(b,c,-1,'&');
				return true;
			}
			if(x_schla == dx_board)
			{	
				turn(b,c,1,'&');
				return true;
			}
			turn(b,c,(rand() % 2) * 2 - 1,'&');
			return true;
		}
		turn(b,c,-dx_schla,'&');
		return true;
	}
	if(x_schla == dx_board)
	{
		if(dy_schla == 0)
		{
			if(y_schla == y_board)
			{
				turn(b,c,1,'&');
				return true;
			}
			if(y_schla == dy_board)
			{
				turn(b,c,-1,'&');
				return true;
			}
			turn(b,c,(rand() % 2) * 2 - 1,'&');
			return true;
		}
		turn(b,c,dy_schla,'&');
		return true;
	}
	if(x_schla == x_board)
	{
		if(dy_schla == 0)
		{
			if(y_schla == y_board)
			{
				turn(b,c,-1,'&');
				return true;
			}
			if(y_schla == dy_board)
			{
				turn(b,c,1,'&');
				return true;
			}
			turn(b,c,(rand() % 2) * 2 - 1,'&');
			return true;
		}
		turn(b,c,-dy_schla,'&');
		return true;
	}
	return false;
}

/*
*	\fn bool schlanga_corner(board* b, snake* c))
* \brief éviter les coins (zone critique )
* \param board* plateau du jeu pour opérer
* \param snake* schlanga
* \return true s'il a opéré, false sinon
*/
bool schlanga_corner(board* b, snake* c)
{
	int x_board = 1;
	int y_board = 1;

	int dx_board = b->x - 2;
	int dy_board = b->y - 2;

	snake head = (*c);

	int schlanga_dir_x = head->x - head->next->x;
	int schlanga_dir_y = head->y - head->next->y;

	if(x_board == head->x && y_board == head->y)
	{
		if(schlanga_dir_x)
		{
			turn(b,c,-1,'&');
		}
		if(schlanga_dir_y)
		{
			turn(b,c,1,'&');
		}
		return true;
	}
	if(x_board == head->x && dy_board == head->y)
	{
		if(schlanga_dir_x)
		{
			turn(b,c,1,'&');
		}
		if(schlanga_dir_y)
		{
			turn(b,c,-1,'&');	
		}
		return true;
	}
	if(dx_board == head->x && y_board == head->y)
	{
		if(schlanga_dir_x)
		{
			turn(b,c,1,'&');
		}
		if(schlanga_dir_y)
		{
			turn(b,c,-1,'&');
		}
		return true;
	}
	if(dx_board == head->x && dy_board == head->y)
	{
		if(schlanga_dir_x)
		{
			turn(b,c,-1,'&');
		}
		if(schlanga_dir_y)
		{
			turn(b,c,1,'&');
		}
		return true;
	}
	return false;
}

/*
*	\fn bool schlanga_next_to(board* b, const snake *s, snake *c)
* \brief détecter si schlanga avance parallèlement et à côté du le snake
* \param board* plateau du jeu pour opérer
* \param snake* snake
* \param snake* schlanga
* \return true s'il a opéré, false sinon
*/
bool schlanga_next_to(board* b, const snake *s, snake *c)
{

	int x_board = 1;
	int y_board = 1;

	int dx_board = b->x - 2;
	int dy_board = b->y - 2;

	int schlanga_x = (*c)->x;
	int schlanga_y = (*c)->y;

	int schlanga_dir_x = (*c)->x - (*c)->next->x;
	int schlanga_dir_y = (*c)->y - (*c)->next->y;

	if(schlanga_x == x_board)
	{
		if(schlanga_dir_x == 0)
		{
			if(exist_in_snake(s,schlanga_x+1,schlanga_y))
			{
				move(b,c,'&');
				return true;
			}
		}
	}
	if(schlanga_x == dx_board)
	{
		if(schlanga_dir_x == 0)
		{
			if(exist_in_snake(s,schlanga_x-1,schlanga_y))
			{
				move(b,c,'&');
				return true;
			}
		}
	}
	if(schlanga_y == y_board)
	{
		if(schlanga_dir_y == 0)
		{
			if(exist_in_snake(s,schlanga_x,schlanga_y+1))
			{
				move(b,c,'&');
				return true;
			}
		}
	}
	if(schlanga_y == dy_board)
	{
		if(schlanga_dir_y == 0)
		{
			if(exist_in_snake(s,schlanga_x,schlanga_y-1))
			{
				move(b,c,'&');
				return true;
			}
		}
	}
	return false;
}
int min_abs(int a, int b)
{
	if(a < 0)
		a = -a;
	if(b < 0)
		b = -b;
	return (a < b)? a : b;
}
/*
*	\fn void schlanga_ia(board* b, const snake* s, snake* c, int len)
* \brief opérer avec l'ensemble des schlanga-fonction
* \param board* plateau du jeu pour opérer
* \param snake* snake
* \param snake* schlanga
* \param int lenght du snake
*/
void schlanga_ia(board* b, const snake* s, snake* c, int len)
{
	static int code = CODE_NULL;
	static int count = 0;
	static int corner = 0;
	int x_board = 1;
	int y_board = 1;

	int dx_board = b->x - 2;
	int dy_board = b->y - 2;

	int schlanga_x = (*c)->x;
	int schlanga_y = (*c)->y;

	int schlanga_dir_x = (*c)->x - (*c)->next->x;
	int schlanga_dir_y = (*c)->y - (*c)->next->y;

	if(schlanga_next_to(b,s,c))
	{
		code = CODE_NEXT_TO;
		return;
	}
	if(schlanga_corner(b,c))
	{
		code = CODE_CORNER;
		corner = len/2-2;
		return;
	}
	if(corner)
	{
		if(schlanga_bow(b,s,c))
		{
			corner = 0;
			code = CODE_BOW;
		}
		else
		{
			move(b,c,'&');
			--corner;
		}
		return;
	}
	if(code != CODE_WALL)
	{
		if(schlanga_wall(b,c))
		{
			code = CODE_WALL;
			unsigned m = 0;
			if(schlanga_dir_x == 0)
			{
				m = min_abs(schlanga_x - x_board, schlanga_x - dx_board);
			}
			if(schlanga_dir_y == 0)
			{
				m = min_abs(schlanga_y - y_board, schlanga_y - dy_board);
			}
			if(m != 0)
			{
				count = rand() % m;
			}
			return;
		}
	}
	else
	{
		if(count == 0)
		{
			code = CODE_NULL;
		}
		else
		{
			if(schlanga_bow(b,s,c))
			{
				code = CODE_BOW;
				return;
			}
			else
			{
				move(b,c,'&');
				--count;
				return;
			}
		}
	}
	if(schlanga_bow(b,s,c))
	{
		code = CODE_BOW;
		return;
	}
	move(b,c,'&');
}