#include "../INCLUDE/snakelib.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
int main() {
	srand(time(0));
	printf("\n\n\n<<< test pour les fonctions type board_# >>>\n\n\n");
	board b = board_init(31,16);
	printf("<<< test board_init | tabX=31 | tabY=16 >>>\n\n\n");
	printf("<<< test board_print >>>\n");
	board_print(&b);
	printf("<<< test board_size_x & board_size_y >>>\n\n\n");
	printf("[size_x=%d : size_y=%d]\n",board_size_x(&b),board_size_y(&b));
	printf("<<< test board_set & board_get >>>\n\n\n");
	board_set(&b,5,9,'@');
	printf("<<< board_set(&b,5,9,'@') >>>\n\n\n");
	board_print(&b);
	printf("<<< board_get(&b,5,9) >>>\n\n\n");
	printf("char=%c\n\n\n\n",board_get(&b,5,9));
	int x,y;
	printf("<<< test board_apple(&b, &x, &y) >>>\n\n\n");
	board_apple(&b, &x, &y);
	board_print(&b);
	printf("apple coord [x=%d:y=%d]\n\n\n",x,y);
	board_apple(&b, &x, &y);
	board_print(&b);
	printf("apple coord [x=%d:y=%d]\n\n\n",x,y);
	board_free(&b);
	printf("\n\n\n<<< test pour les fonctions type snake_# >>>\n\n\n");
	printf("<<< test snake_init | len=5 >>>\n\n\n");
	b=board_init(31,16);
	snake s = 0;
	snake_init(&b,&s,5,'@');
	printf("<<< test snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< snake print avec board_print >>>\n\n\n");
	board_print(&b);
	printf("<<< test move >>>\n\n\n");
	move(&b,&s,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	move(&b,&s,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	move(&b,&s,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	printf("<<< test turn >>>\n\n\n");
	turn(&b,&s,-1,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	turn(&b,&s,1,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	turn(&b,&s,-1,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	turn(&b,&s,1,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	board_free(&b);
	snake_free(&s);
	printf("\n\n\n<<< test choc_wall >>>\n\n\n");
	b=board_init(31,16);
	snake_init(&b,&s,5,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	bool test=false;
	while(!test) {
		move(&b,&s,'@');
		test=choc_wall(&b,&s);
		printf("<<< snake_print >>>\n\n\n");
		snake_print(&s);
		printf("<<< board_print >>>\n\n\n");
		board_print(&b);
		if(test)
			printf("<<< choc_wall=>true >>>\n\n\n");
		else
			printf("<<< choc_wall=>false >>>\n\n\n");
	}
	board_free(&b);
	snake_free(&s);
	printf("\n\n\n<<< test choc_snake >>>\n\n\n");
	b=board_init(31,16);
	snake_init(&b,&s,5,'@');
	printf("<<< snake_print >>>\n\n\n");
	snake_print(&s);
	printf("<<< board_print >>>\n\n\n");
	board_print(&b);
	test=false;
	while(!test) {
		turn(&b,&s,-1,'@');
		test=choc_snake(&s,&s);
		printf("<<< snake_print >>>\n\n\n");
		snake_print(&s);
		printf("<<< board_print >>>\n\n\n");
		board_print(&b);
		if(test)
			printf("<<< choc_snake=>true >>>\n\n\n");
		else
			printf("<<< choc_snake=>false >>>\n\n\n");
	}
	board_free(&b);
	snake_free(&s);
	return 0;
}