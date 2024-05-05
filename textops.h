#define ESCAPE '\e'
#define NEWLINE '\n'

void insert_to_list(dNode **curr_node, line *curr_line, char input) {
	dNode *new_node=malloc(sizeof(dNode)); 
	curr_line->size+=1;
	if((*curr_node)->prev==NULL) { // if current node is at the start of a line
		new_node->prev=NULL;
		curr_line->start=new_node;
	}
	else { // else it's at the middle or end of a line
		new_node->prev=(*curr_node)->prev;
		(*curr_node)->prev->next=new_node; 
	}
	new_node->next=(*curr_node);
	new_node->ch=input;
	(*curr_node)->prev=new_node; // update current node's previous
}

void backspace(line **curr_line, dNode **curr_node) {
	if((*curr_node)->prev==NULL) { // if current node is at start of line
		if((*curr_line)->prev_line==NULL) return; // if current line is at the beginning of a file do nothing
		line *temp_line=*curr_line; 
		*curr_line=(*curr_line)->prev_line;
		(*curr_line)->next_line=temp_line->next_line;
		(*curr_line)->size+=temp_line->size; // update size of current line
		(*curr_node)->prev=(*curr_line)->end; // set the prev node of current node to previous line's end
		if ((*curr_line)->end==NULL) (*curr_line)->start=temp_line->start; // if current line was empty just swap its start
		else (*curr_line)->end->next=temp_line->start; // replace current line's end with old line's start
		(*curr_line)->end=temp_line->end; // update current line's end
		free(temp_line);
	}	
	else {
		dNode *temp=(*curr_node)->prev;
		(*curr_node)->prev=temp->prev;
		if((*curr_node)->next==NULL) (*curr_line)->end==temp->prev; // if current node is the end of a line, change line end
		if(temp->prev==NULL) (*curr_line)->start=*curr_node; //if no node before temp assign current as start
		else temp->prev->next=*curr_node;
		(*curr_line)->size-=1;
		free(temp);
	}
}

int count_size(dNode *head) {
	int size=0;
	while(head->ch!='\n') {
		size++;
		head=head->next;
	}
	return size;
}

void insert_newline(dNode **curr_node, line **curr_line) {
	line *new_line=malloc(sizeof(line));
	if((*curr_line)->prev_line==NULL) new_line->prev_line=NULL; // if current line's is at the beginning of file
	else { 
		new_line->prev_line=(*curr_line)->prev_line;
		(*curr_line)->prev_line->next_line=new_line;
	}
	dNode *new_node=malloc(sizeof(dNode));
	new_node->ch='\n';
	new_node->next=NULL;

	if((*curr_node)->prev==NULL) {
		new_line->start=new_node;
		new_line->end=new_node->prev=NULL;
	}
	else {
		new_line->start=(*curr_line)->start;
		new_line->end=(*curr_node)->prev;
		new_line->end->next=new_node;
		(*curr_line)->start=*curr_node;
		(*curr_node)->prev=NULL;
	}
	new_line->next_line=(*curr_line);
	(*curr_line)->prev_line=new_line;
	new_line->size=count_size(new_line->start);
	(*curr_line)->size-=new_line->size;
}

void insert_mode(int *row, int *col, line **curr_line, dNode **curr_node) {
	unsigned int key; // vairable is unsigned int to increase range, since characters like backspace are greater than sizeof(char)
	while((key=getch())!=ESCAPE) {
		switch(key) {
			case KEY_BACKSPACE:
				if((*curr_line)->prev_line==NULL && (*curr_node)->prev==NULL) break; // if we are at the beginning of a file do nothing
				if((*curr_node)->prev==NULL) {
					*col=(*curr_line)->prev_line->size;
					deleteln(); // delete's current line using ncurses func
					move(--(*row), 0);
				}
				else {
					delch();
					mvdelch(*row, --(*col));
				}
				backspace(curr_line, curr_node);
				display(*curr_line);
				move(*row, *col);
				break;
			case NEWLINE:
				insert_newline(curr_node, curr_line);
				clrtobot();
				display((*curr_line)->prev_line);
				*col=0; // update column
				move(++(*row), *col);	
				break;
			default:
				insert_to_list(curr_node, *curr_line, key);
				insch(key); // insert char to terminal using ncurses func	
				move(*row, ++(*col));
		}
	}
	if((*curr_node)->ch=='\n') mv_left(row, col, curr_line, curr_node);
}

/*
				move(*row, 0); // move cursor to start of line for reprinting
				displayLine((*curr_line)->start); //re-print the current line after change
	if((*curr_node)->prev==NULL) { // if inserting newline at the start of a line
		new_line->start=new_node;
		new_line->end=new_node->prev=NULL;
	}
	else if((*curr_node)->ch=='\n') { // if inserting newline from end of a line
		new_line->start=new_node;
		new_line->size=0;
		new_line->end=new_node->prev=NULL;
		new_line->prev_line=*curr_line;	
		new_line->next_line=(*curr_line)->next_line;
		(*curr_line)->next_line=new_line;
		*curr_line=(*curr_line)->next_line;
		return;
	}
	else { // else newline is inserted somewhere in the middle
		(*curr_node)->prev->next=new_node;
		new_line->start=(*curr_line)->start;
		(*curr_line)->start=(*curr_node);
		(*curr_node)->prev=NULL;
	}
	*curr_line=(*curr_line)->next_line;
	new_line->next_line=(*curr_line);
	new_line->size=count_size(new_line->start); //update sizes
	(*curr_line)->size-=new_line->size;
 */
