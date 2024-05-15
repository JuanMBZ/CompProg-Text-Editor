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
		if(temp_line->next_line!=NULL) temp_line->next_line->prev_line=buff->curr_line; // sets the next line of temp line to prev line
		(buff->curr_line)->next_line=temp_line->next_line;
		(buff->curr_line)->width+=temp_line->width;
		(buff->curr_line)->size+=temp_line->size; // update size of current line
		(buff->curr_node)->prev=(buff->curr_line)->end; // set the prev node of current node to previous line's end
		if ((buff->curr_line)->end==NULL) (buff->curr_line)->start=temp_line->start; // if current line was empty just swap its start
		else (buff->curr_line)->end->next=temp_line->start; // replace current line's end with old line's start
		(buff->curr_line)->end=temp_line->end; // update current line's end
		free(temp_line);
	}	
	else {
		if(buff->curr_node->ch=='\n') buff->curr_line->width-=8;
		else buff->curr_line->width-=1;
		dNode *temp=(buff->curr_node)->prev;
		(buff->curr_node)->prev=temp->prev;
		if((buff->curr_node)->next==NULL) (buff->curr_line)->end==temp->prev; // if current node is the end of a line, change line end
		if(temp->prev==NULL) (buff->curr_line)->start=buff->curr_node; //if no node before temp assign current as start
		else temp->prev->next=buff->curr_node;
		(buff->curr_line)->size-=1;
		free(temp);
	}
}

int count_width(dNode *head) {
	int width=0;
	while(head->ch!='\n') {
		if(head->ch=='\t') width+=8;
		else width++;
		head=head->next;
	}
	return width;
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

	if((buff->curr_node)->prev==NULL) { //if at the start of a line
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

	new_line->width=count_width(new_line->start); //update new line and current line's width and size
	(buff->curr_line)->width-=new_line->width;
	new_line->size=count_size(new_line->start);
	(buff->curr_line)->size-=new_line->size;
}
void append(buffer *buff) {
	if(buff->curr_node->ch=='\n') return;
	else if(buff->curr_node->next->ch=='\n') {
		buff->curr_node=buff->curr_node->next;
		if(buff->col==(buff->max_col-1)) {
			buff->col=0;
			buff->row+=1;
		}
		else buff->col+=1;
		move(buff->row, buff->col);
	}
	else mv_right(buff);
}

void insert_mode(buffer *buff) {
	unsigned int key; // vairable is unsigned int to increase range, since characters like backspace are greater than sizeof(char)
	while((key=getch())!=ESCAPE) {
		switch(key) {
			case KEY_UP: mv_up(buff); break;
			case KEY_DOWN: mv_down(buff); break;
			case KEY_LEFT: mv_left(buff); break;
			case KEY_RIGHT: append(buff); break;
			case KEY_BACKSPACE:
				if((buff->curr_line)->prev_line==NULL && (buff->curr_node)->prev==NULL) break; // if we are at the beginning of a file do nothing
				if((buff->curr_node)->prev==NULL) { // if at beginning of line
					buff->col=(buff->curr_line->prev_line->width)%(buff->max_col);
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

void delete_char(buffer *buff) {
	if(buff->curr_node->ch=='\n') return; // If line is empty do nothing
	dNode *temp=buff->curr_node;
	buff->curr_node=buff->curr_node->next;
	buff->curr_node->prev=temp->prev;
	if(temp->ch=='\t') buff->curr_line->width-=8;
	else buff->curr_line->width-=1;
	if(temp->prev==NULL) buff->curr_line->start=buff->curr_node; //if deleting a node brings us to start of line, update start
	else temp->prev->next=buff->curr_node;
	free(temp);
	display(buff);
	move(buff->row, buff->col);
}

void delete_line(buffer *buff) {
	if(buff->curr_line->prev_line==NULL && buff->curr_line->next_line==NULL) { // if we are at start of file and last line
		if(buff->curr_line->start->ch=='\n') return; //if file is empty do nothing
		dNode *temp_node=buff->curr_line->start;
		while(temp_node->ch!='\n') { //if current line contains something, empty it
			temp_node=temp_node->next;
			free(temp_node->prev);
		}
		buff->curr_line->start=buff->curr_node=temp_node;
		buff->curr_node->next=buff->curr_node->prev=NULL;
		display(buff);
		move(0,0);
		return;
	}
	line *temp=buff->curr_line;
	int dist=count_char_dist(buff), count=0;
	if(temp->next_line==NULL) {
		buff->curr_line=buff->curr_line->prev_line;
		buff->curr_line->next_line=NULL;
		buff->row-=1;
	}
	else {
		buff->curr_line=buff->curr_line->next_line;
		if(temp==buff->top) buff->top=buff->curr_line; // update top if deleted temp is top
		if(temp==buff->head) buff->head=buff->curr_line; // if we were at head of buffer, update buffer head
		else temp->prev_line->next_line=buff->curr_line;
		buff->curr_line->prev_line=temp->prev_line;
	}

	buff->curr_node=buff->curr_line->start;
	while(count<buff->col && buff->curr_node->next->ch!='\n') { //jump to the same node from same column of deleted line
		if(buff->curr_node->ch=='\t') count+=8;
		else count++;
		buff->curr_node=buff->curr_node->next;	
	}
	if(count<buff->col) buff->col=count; // update col if jumped line<deleted line
	
	if(dist>buff->max_col) { //accounting for wrapped lines
		buff->row-=(dist/buff->max_col);
		if(dist%buff->max_col==0) buff->row+=1;
	}
	
	dNode *temp_node=temp->start;
	while(temp_node->ch!='\n') { //delete all nodes contained in the line
		temp_node=temp_node->next;
		free(temp_node->prev);
	} free(temp_node); //free final node
	free(temp); //finally, free line
	display(buff);
	move(buff->row,buff->col);
}
