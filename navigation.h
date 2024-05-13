
void mv_up(int *row, int *col, buffer *buff) {
	if((buff->curr_line)->prev_line==NULL) return; // if at the start of a file
	buff->curr_line=(buff->curr_line)->prev_line;
	int i=0;
	buff->curr_node=(buff->curr_line)->start;
	if((buff->curr_node)->next!=NULL) {
		while((buff->curr_node)->next->next!=NULL && i<*col) { // while not at the end of the next line and while not at the prev column
			(buff->curr_node)=(buff->curr_node)->next;
			i++;
		}
	}
	(*row)--;
	if(i<*col) *col=i;
	move(*row, *col);
}

void mv_down(int *row, int *col, buffer *buff) {
	if((buff->curr_line)->next_line==NULL) return;
	buff->curr_line=(buff->curr_line)->next_line;
	int i=0;
	buff->curr_node=(buff->curr_line)->start;
	if((buff->curr_node)->next!=NULL) {
		while((buff->curr_node)->next->next!=NULL && i<*col) {
			(buff->curr_node)=(buff->curr_node)->next;
			i++;
		}
	}
	(*row)++;
	if(i<*col) *col=i;
	move(*row, *col);
}

void mv_right(int *row, int *col, buffer *buff) {
	if((buff->curr_node)->ch=='\n') return; // if current node is a newline char
	if((buff->curr_node)->next->next==NULL) return; // if current node is at the end, do nothing
	buff->curr_node=(buff->curr_node)->next;
	*col+=1;
	move(*row, *col);
}

void mv_left(int *row, int *col, buffer *buff) {
	if((buff->curr_node)->prev==NULL) return;
	buff->curr_node=(buff->curr_node)->prev;
	*col-=1;
	move(*row, *col);
}
