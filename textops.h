#define ESCAPE '\e'
#define NEWLINE '\n'

void insert_to_list(buffer *buff, char input) {
	dNode *new_node=malloc(sizeof(dNode)); 
	buff->curr_line->size+=1;
	if(input=='\t') buff->curr_line->width+=8;
	else buff->curr_line->width+=1;
	if((buff->curr_node)->prev==NULL) { // if current node is at the start of a line
		new_node->prev=NULL;
		buff->curr_line->start=new_node;
	}
	else { // else it's at the middle or end of a line
		new_node->prev=(buff->curr_node)->prev;
		(buff->curr_node)->prev->next=new_node; 
	}
	new_node->next=(buff->curr_node);
	new_node->ch=input;
	(buff->curr_node)->prev=new_node; // update current node's previous
}

void backspace(buffer *buff) {
	if((buff->curr_node)->prev==NULL) { // if current node is at start of line
		if((buff->curr_line)->prev_line==NULL) return; // if current line is at the beginning of a file do nothing
		line *temp_line=buff->curr_line; 
		buff->curr_line=(buff->curr_line)->prev_line;
		(buff->curr_line)->next_line=temp_line->next_line;
		(buff->curr_line)->size+=temp_line->size; // update size of current line
		(buff->curr_node)->prev=(buff->curr_line)->end; // set the prev node of current node to previous line's end
		if ((buff->curr_line)->end==NULL) (buff->curr_line)->start=temp_line->start; // if current line was empty just swap its start
		else (buff->curr_line)->end->next=temp_line->start; // replace current line's end with old line's start
		(buff->curr_line)->end=temp_line->end; // update current line's end
		free(temp_line);
	}	
	else {
		dNode *temp=(buff->curr_node)->prev;
		(buff->curr_node)->prev=temp->prev;
		if((buff->curr_node)->next==NULL) (buff->curr_line)->end==temp->prev; // if current node is the end of a line, change line end
		if(temp->prev==NULL) (buff->curr_line)->start=buff->curr_node; //if no node before temp assign current as start
		else temp->prev->next=buff->curr_node;
		(buff->curr_line)->size-=1;
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

void insert_newline(buffer *buff) {
	line *new_line=malloc(sizeof(line));
	if((buff->curr_line)->prev_line==NULL){
		new_line->prev_line=NULL; // if current line's is at the beginning of file
		buff->head=new_line;
	}
	else { 
		new_line->prev_line=(buff->curr_line)->prev_line;
		(buff->curr_line)->prev_line->next_line=new_line;
	}
	if(buff->curr_line==buff->top) { //if current line is at the top of the display
		buff->top=new_line;
	}
	dNode *new_node=malloc(sizeof(dNode));
	new_node->ch='\n';
	new_node->next=NULL;

	if((buff->curr_node)->prev==NULL) {
		new_line->start=new_node;
		new_line->end=new_node->prev=NULL;
	}
	else {
		new_line->start=(buff->curr_line)->start;
		new_line->end=(buff->curr_node)->prev;
		new_line->end->next=new_node;
		(buff->curr_line)->start=buff->curr_node;
		(buff->curr_node)->prev=NULL;
	}
	new_line->next_line=(buff->curr_line);
	(buff->curr_line)->prev_line=new_line;
	new_line->size=count_size(new_line->start);
	(buff->curr_line)->size-=new_line->size;
}

void insert_mode(buffer *buff) {
	unsigned int key; // vairable is unsigned int to increase range, since characters like backspace are greater than sizeof(char)
	while((key=getch())!=ESCAPE) {
		switch(key) {
			case KEY_BACKSPACE:
				if((buff->curr_line)->prev_line==NULL && (buff->curr_node)->prev==NULL) break; // if we are at the beginning of a file do nothing
				if((buff->curr_node)->prev==NULL) { // if at beginning of line
					buff->col=(buff->curr_line->prev_line->size)%(buff->max_col);
					move(--(buff->row), 0);
				}
				else {
					delch();
					mvdelch(buff->row, --(buff->col));
				}
				backspace(buff);
				display(buff);
				move(buff->row, buff->col);
				break;
			case NEWLINE:
				insert_newline(buff);
				clrtobot();
				display(buff);
				buff->col=0; // update column
				move(++(buff->row), buff->col);	
				break;
			default:
				insert_to_list(buff, key);
				display(buff);
				if(key=='\t') buff->col+=8;
				else buff->col++;
				if((buff->col)==(buff->max_col)-1) {
					buff->col=0;
					(buff->row)++;
				}
				move(buff->row, buff->col);
		}
	}
	if((buff->curr_node)->ch=='\n') mv_left(buff);
}

//void delete_char(int buff->col, line buff->curr_line, dNode *buff->curr_node) {
//}

