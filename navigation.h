
void mv_up(int *row, int *col, line **curr_line, dNode **curr_node) {
	if((*curr_line)->prev_line==NULL) return; // if at the start of a file
	*curr_line=(*curr_line)->prev_line;
	if((*curr_line)->start==NULL) { // if empty line
		move(--(*row), 0);
		*curr_node=NULL;
		return;
	}
	int i=0;
	*curr_node=(*curr_line)->start;
	while((*curr_node)->next!=NULL && i<*col) { // while not at the end of the next line and while not at the prev column
		(*curr_node)=(*curr_node)->next;
		i++;
	}
	(*row)--;
	if(i<*col) move(*row, i);
	else move(*row, *col);
}

void mv_down(int *row, int *col, line **curr_line, dNode **curr_node) {
	if((*curr_line)->next_line==NULL) return;
	*curr_line=(*curr_line)->next_line;
	if((*curr_line)->start==NULL) {
		move(++(*row), 0);
		*curr_node=NULL;
		return;
	}
	int i=0;
	*curr_node=(*curr_line)->start;
	while((*curr_node)->next!=NULL && i<*col) {
		(*curr_node)=(*curr_node)->next;
		i++;
	}
	(*row)++;
	if(i<*col) move(*row, i);
	else move(*row, *col);
}

void mv_right(int *row, int *col, line **curr_line, dNode **curr_node) {
	if((*curr_line)->start==NULL) return; // if empty line
	if((*curr_node)->next==NULL) return; // if current node is at the end
	*curr_node=(*curr_node)->next;
	*col+=1;
	move(*row, *col);
}

void mv_left(int *row, int *col, line **curr_line, dNode **curr_node) {
	if((*curr_line)->start==NULL) return; // if empty line
	if((*curr_node)->prev==NULL) return;
	*curr_node=(*curr_node)->prev;
	*col-=1;
	move(*row, *col);
}
