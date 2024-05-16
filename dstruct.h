#include <stdio.h>
#include <stdlib.h>

/* header file for node struct and creating/displaying heads/list
 */

typedef struct dNode { // data struct to store each individual char
	char ch;
	struct dNode *prev;
	struct dNode *next;
	int color;
} dNode;

typedef struct line{ // data struct for connectin lines and info about the lines
	dNode *start;
	dNode *end;
	struct line *prev_line;
	struct line *next_line;
	int size; // number of ascii chars
	int width; // length of line in display (made mainly to account for tab chars)
} line;

typedef struct buffer { //contains info on the contents of the text editor and info on the display
	line *curr_line;
	dNode *curr_node;
	char *file_name;
	
	line *top, *bot;
	line *head, *tail;
	line *copy_line_head, *copy_line_tail;
	dNode *copy_head, *copy_tail;
	
	int row, col; // coordinates of the cursor on the display
	int max_row, max_col; // max y and x for the display, must be static
	int copy_flag; // if 0=empty, 1=dNodes, 2=lines
} buffer;

line *createNewList() { // Initialize an empty buffer for a new file
	dNode *curr=malloc(sizeof(dNode));
	line *head=malloc(sizeof(line));
	head->next_line=head->prev_line=NULL;
	head->start=curr;
	head->size=head->width=0;
	curr->next=curr->prev=head->end=NULL;
	curr->ch='\n';
	return head;
}

line* createList(FILE *file_in) {
	dNode *curr;
	line *head, *curr_line;
	int n=0, w=0;
	char c;
	//Initialize line start
	head=(line*)malloc(sizeof(line));
	curr_line=head;
	curr_line->next_line=curr_line->prev_line=NULL;
	while(fscanf(file_in, "%c", &c)!=EOF) {//Iterate until EOF
		if(n==0) { // if start of new line
			curr_line->start=(dNode*) malloc(sizeof(dNode));
			curr=curr_line->start; // set the current line's start as the current char
			curr->prev=NULL;
			curr->ch=c;
			curr->color=-0;
		}
		else { // if at the middle of a line
			curr->next=(dNode*) malloc(sizeof(dNode));
			(curr->next)->prev=curr;
			curr=curr->next;
			curr->ch=c;
			curr->color=-0;
		}
		if(c=='\t') w+=8;
		else w++; 
		n++; //increment line width and size 
		if(c=='\n') { // if at the end of a line
			curr_line->end=curr->prev; // set end of line
			curr_line->size=n-1; //record line size
			curr_line->width=w-1;
			curr_line->next_line=(line*)malloc(sizeof(line));
			(curr_line->next_line)->prev_line=curr_line;
			curr_line=curr_line->next_line;
			w=n=0; //restart count
			curr->next=NULL; //assigns null to last node
		}
		
	}
	if(curr_line->prev_line==NULL) { //if file was empty or had one line
		curr_line->start=(dNode*) malloc(sizeof(dNode));
		curr_line->end=curr_line->start->prev=curr_line->start->next=NULL;
		curr_line->start->ch='\n';
		curr_line->size=0;
	}
	else {
		curr_line=curr_line->prev_line;
		free(curr_line->next_line); // free excess line because tested text files end with a \n
		curr_line->next_line=NULL;
		curr->next=NULL;
	}
	return head;
}

void displayLine(dNode* head) {
	dNode *curr=head;
	int row, col;
	getyx(stdscr, row, col);
	move(row, 0);
	while(curr!=NULL) {
		addch(curr->ch);
		curr=curr->next;
	}
}

void display(buffer *buff) {
	line *curr=buff->top;
	dNode *curr_node;
	int count=0, row, col;
	move(0, 0);
	clrtobot();
	while(curr!=NULL) { // traverse lines until it finds NULL
		curr_node=curr->start;
		while(curr_node!=NULL) {
			addch(curr_node->ch | COLOR_PAIR(curr_node->color));
			curr_node=curr_node->next;
		}
		getyx(stdscr, row, col);
		if(row==buff->max_row-1) {
			buff->bot=curr;
			break;
		}
		if(curr->next_line==NULL) buff->bot=curr; //check if current line is at eof
		curr=curr->next_line;
	}
	move(buff->max_row-1, 0);
	clrtoeol();
	attron(COLOR_PAIR(1));
	printw("%-*s", buff->max_col-1, buff->file_name);
	attroff(COLOR_PAIR(1));
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
