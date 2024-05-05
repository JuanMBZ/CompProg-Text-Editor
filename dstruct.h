#include <stdio.h>
#include <stdlib.h>

/* header file for node struct and creating/displaying heads/list
 */

typedef struct dNode {
	char ch;
	struct dNode *prev;
	struct dNode *next;
} dNode;

typedef struct line{
	dNode *start;
	dNode *end;
	struct line *prev_line;
	struct line *next_line;
	unsigned int size;
} line;


line* createList(FILE *file_in) {
	dNode *curr;
	line *head, *curr_line;
	int n=0;
	char c;
	//Initialize line start
	head=(line*)malloc(sizeof(line));
	curr_line=head;
	curr_line->prev_line=NULL;

	while(fscanf(file_in, "%c", &c)!=EOF) {//Iterate until EOF
		if(n==0) { // if start of new line
			curr_line->start=(dNode*) malloc(sizeof(dNode));
			curr=curr_line->start; // set the current line's start as the current char
			curr->prev=NULL;
			curr->ch=c;
			n++;
		}
		else { // if at the middle of a line
			curr->next=(dNode*) malloc(sizeof(dNode));
			(curr->next)->prev=curr;
			curr=curr->next;
			curr->ch=c;
			n++;
		}
		if(c=='\n') { // if at the end of a line
			curr_line->end=curr->prev; // set end of line
			curr_line->size=n-1; //record line size
			curr_line->next_line=(line*)malloc(sizeof(line));
			(curr_line->next_line)->prev_line=curr_line;
			curr_line=curr_line->next_line;
			n=0; //restart count
			curr->next=NULL; //assigns null to last node
		}
		
	}
	curr_line=curr_line->prev_line;
	free(curr_line->next_line); // free excess line because linux text files end with a \n
	curr_line->next_line=NULL;
	curr->next=NULL;
	return head;
}

void displayLine(dNode* head) {
	dNode *curr=head;
	while(curr!=NULL) {
		addch(curr->ch);
		refresh(); // used to immediately output char to screen
		curr=curr->next;
	}
}

void display(line *head) {
	line *curr=head;
	int count=0, row, col;
	getyx(stdscr, row, col);
	move(row, 0);
	while(curr!=NULL) { // traverse lines until it finds NULL
		displayLine(curr->start);
		refresh();
//		if(curr->next_line!=NULL) { addch('\n'); refresh();}
		count++;
		curr=curr->next_line;
	}
	printw("\nLines printed: %d", count);
	getmaxyx(stdscr, row, col);
	mvprintw(row-1, 0, "Rows: %d Columns: %d", row, col);
	refresh();
}

void write_line(dNode* head, FILE *out) {
	dNode *curr=head;
	while(curr!=NULL) {
		fputc(curr->ch, out);
		curr=curr->next;
	}
}

void save(line *top, FILE *out) {
	line *curr_line=top;
	while(curr_line!=NULL) {
		write_line(curr_line->start, out);
		curr_line=curr_line->next_line;
	}
}
