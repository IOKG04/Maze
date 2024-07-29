#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "display.h"

int main(int argc, char **argv){
    setup_screen(80, 25);

    draw_screen();

    char inp;
    srand(time(NULL));
    do{
	inp = getchar();
	size_t x = rand() % 80, y = rand() % 25;
	set_pixel(x, y, inp);
	draw_screen();
    } while(inp != 'q');

    revert_screen();
    exit(0);
}
