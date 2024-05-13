#include <stdio.h>
#include <ncurses.h>
#include "dstruct.h"
#include "navigation.h"
#include "textops.h"

#define UP 'k'
#define DOWN 'j'
#define RIGHT 'l'
#define LEFT 'h'
#define INSERT 'i'
#define DELETE_CHAR 'x'
#define DELETE_LINE 'd'

int main(int argc, char *argv[]) {
	unsigned int ch; // variable is unsigned int to increase range
	FILE *fiptr, *foptr;
	buffer buff;

	if(argc!=2) { printf("Too many/few arguments.\n"); exit(0);}
	fiptr=fopen(argv[1], "r");

	initscr(); // Starts curses mode 
	keypad(stdscr, TRUE); // Enables use of keypad and F keys
	raw(); // Enters raw mode which disables line buffering and control keys
	noecho(); // Disables echo while doing getch() 
	
	getmaxyx(stdscr, buff.max_row, buff.max_col); // get number of rows and column

	buff.head=createList(fiptr);
	buff.top=buff.curr_line=buff.head;
	buff.curr_node=buff.head->start;
	display(&buff);
	move(0,0); //move cursor to start

	while(1) {
		ch=getch();
		switch(ch) {
			case UP:
				mv_up(&buff); break;
			case DOWN:
				mv_down(&buff); break;
			case RIGHT:
				mv_right(&buff); break;
			case LEFT:
				mv_left(&buff); break;
			case INSERT:
				insert_mode(&buff);
				break;
			case DELETE_CHAR:
				break;
			case DELETE_LINE:
				break;
		}
		if(ch=='q') break;
	}

	fclose(fiptr);
	mvprintw(buff.max_row-2, 0, "Last Char: %c Last Line Size: %d", (buff.curr_node)->ch, (buff.curr_line)->size); // move then print
	mvprintw(buff.max_row-3, 0, "Last Line: ", (buff.curr_node)->ch); 
	displayLine((buff.curr_line)->start);
	mvprintw(buff.max_row-1, 0, "Save contents? (y/n): ");
	refresh(); //print on screen
	ch=getch(); // wait for user input
	
	if(ch=='y') {
		foptr=fopen(argv[1], "w");
		save(buff.head, foptr);
	}
	endwin(); // end curses mode
	
	return 0;
}
