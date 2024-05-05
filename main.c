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

int main(int argc, char *argv[]) {
	int max_row, max_col, row=0, col=0;
	char ch;
	line *top, *curr_line;
	dNode *curr_node;
	FILE *fiptr, *foptr;
	
	if(argc!=2) { printf("Too many/few arguments.\n"); exit(0);}
	fiptr=fopen(argv[1], "r");

	initscr(); // Starts curses mode 
	keypad(stdscr, TRUE); // Enables use of keypad and F keys
	raw(); // Enters raw mode which disables line buffering and control keys
	noecho(); // Disables echo while doing getch() 
	getmaxyx(stdscr, max_row, max_col); // get number of rows and column

	top=createList(fiptr);
	display(top);
	curr_line=top;
	curr_node=top->start;
	mvprintw(max_row-1, 0, "Rows: %d Columns: %d", max_row, max_col); // move then print
	move(0,0); //move cursor to start

	while(1) {
		ch=getch();
		switch(ch) {
			case UP:
				mv_up(&row, &col, &curr_line, &curr_node); break;
			case DOWN:
				mv_down(&row, &col, &curr_line, &curr_node); break;
			case RIGHT:
				if(col>(curr_line->size)-1) col=(curr_line->size)-1; // Update row
				mv_right(&row, &col, &curr_line, &curr_node);
				break;
			case LEFT:
				if(col>(curr_line->size)-1) col=(curr_line->size)-1;
				mv_left(&row, &col, &curr_line, &curr_node);
				break;
			case INSERT:
				if(col>(curr_line->size)-1) col=(curr_line->size)-1;
				insert_mode(&row, &col, &curr_line, &curr_node);
		}
		if(ch=='q') break;
	}

	mvprintw(max_row-2, 0, "Last Char: %c Last Line Size: %d", curr_node->ch, curr_line->size); // move then print
	mvprintw(max_row-3, 0, "Last Line: ", curr_node->ch); 
	displayLine(curr_line->start);

	refresh(); //print on screen
	getch(); // wait for user input
	endwin(); // end curses mode
	
	return 0;
}
