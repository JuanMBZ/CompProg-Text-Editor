#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include "dstruct.h"
#include "navigation.h"
#include "textops.h"
#include "copy.h"
#include "syntax.h"

#define UP 'k'
#define DOWN 'j'
#define RIGHT 'l'
#define LEFT 'h'
#define INSERT 'i'
#define APPEND 'a'
#define WRITE 'w'
#define DELETE_CHAR 'x'
#define DELETE_LINE 'd'
#define SELECT 's'
#define SELECT_LINE 'S'
#define COPY 'c'
#define PASTE 'p'

int main(int argc, char *argv[]) {
	unsigned int ch; // variable is unsigned int to increase range
	FILE *fptr;
	buffer buff;

	if(argc!=2) { printf("Too many/few arguments.\n"); exit(0);}
	fptr=fopen(argv[1], "r");
	if(fptr==NULL) {
		buff.head=createNewList();
		fptr=fopen(argv[1], "w");
	}
	else buff.head=createList(fptr);
	buff.top=buff.curr_line=buff.head;
	buff.curr_node=buff.head->start;
	buff.copy_head=NULL;
	buff.copy_line_head=NULL;
	buff.file_name=argv[1];

	initscr(); // Starts curses mode 
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE); //for highlight
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);

	keypad(stdscr, TRUE); // Enables use of keypad and F keys
	raw(); // Enters raw mode which disables line buffering and control keys
	noecho(); // Disables echo while doing getch() 
	getmaxyx(stdscr, buff.max_row, buff.max_col); // get number of rows and column
	update_syntax(&buff);
	display(&buff);
	move(0,0); //move cursor to start

	while(1) {
		ch=getch();
		switch(ch) {
			case UP: case KEY_UP:
				mv_up(&buff); break;
			case DOWN: case KEY_DOWN:
				mv_down(&buff); break;
			case RIGHT: case KEY_RIGHT:
				mv_right(&buff); break;
			case LEFT: case KEY_LEFT:
				mv_left(&buff); break;
			case INSERT:
				insert_mode(&buff);
				break;
			case APPEND:
				append(&buff);
				insert_mode(&buff);
				break;
			case WRITE:
				attron(COLOR_PAIR(1));
				mvprintw(buff.max_row-1, 0, "Save current contents to file? (y/n): ");
				attroff(COLOR_PAIR(1));
				if((ch=getch())=='y') {
					freopen(argv[1], "w+", fptr); // reopens file with w mode which clears content to avoid multiple writings
					save(buff.head, fptr);
				}
				display(&buff);
				move(buff.row, buff.col);
				break;
			case DELETE_CHAR:
				delete_char(&buff);
				break;
			case DELETE_LINE:
				delete_line(&buff);
				break;
			case SELECT:
				select_mode(&buff);
				break;
			case SELECT_LINE:
				selectLine_mode(&buff);
				break;
			case PASTE:
				paste(&buff);
				break;
		}
		if(ch=='q') break;
	}

//	mvprintw(buff.max_row-2, 0, "Last Char: %c Last Line Size: %d Width: %d Max_row: %d Col: %d", (buff.curr_node)->ch, (buff.curr_line)->size, buff.curr_line->width, buff.max_row, buff.max_col); // debugging info
//	mvprintw(40, 0, "Current Line buffer: "); 
//	displayLine(buff.curr_line->start);
	attron(COLOR_PAIR(1));
	mvprintw(buff.max_row-1, 0, "Save contents? (y/n): ");
	attroff(COLOR_PAIR(1));
	refresh(); //print on screen
	ch=getch(); // wait for user input
	
	if(ch=='y') {
		freopen(argv[1], "w", fptr);
		save(buff.head, fptr);
	}
	endwin(); // end curses mode
	fclose(fptr);
	
	return 0;
}
