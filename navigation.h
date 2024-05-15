
int count_char_dist(buffer *buff) { //returns distance of current node from start counting indents (mainly used for navigation when line width exceeds max columns
	if(buff->curr_node->next==NULL) return buff->curr_line->width;
	if(buff->curr_node->ch=='\n') return 0;
	int pos=1;
	dNode *start=buff->curr_line->start;
	while(start!=buff->curr_node) {
		start=start->next;
		if(start->ch=='\t') pos+=8;
		else pos++;
	}
	return pos;
}

void mv_up(buffer *buff) {
	if((buff->curr_line)->prev_line==NULL) return; // if at the start of a file
	int i=0;
	buff->curr_line=(buff->curr_line)->prev_line;
	buff->curr_node=(buff->curr_line)->start;
	if(buff->curr_line->width>buff->max_col) { // if current line is wrapped (line is longer than display width)
		int overflow;
		overflow=(buff->curr_line->width/buff->max_col);
		if(buff->curr_line->width%buff->max_col==0) overflow-=1;
		buff->row-=(overflow-(count_char_dist(buff)/buff->max_col));
		if((count_char_dist(buff)%buff->max_col)==0) buff->row+=1;
	}
	if((buff->curr_node)->next!=NULL) { //if line is not empty move left until it reaches same position of last line
		while((buff->curr_node)->next->next!=NULL && i<buff->col) {
			if(buff->curr_node->ch=='\t') i+=8;
			else i++;
			(buff->curr_node)=(buff->curr_node)->next;
		}
	}
	buff->row-=1; // move row coordinate up 1 + how many times the line wrapped
	if(i<buff->col) buff->col=i;
	move(buff->row, buff->col);
}

void mv_down(buffer *buff) {
	if((buff->curr_line)->next_line==NULL) return; // if at the start of a file
	if(buff->curr_line->width>buff->max_col) { //if current line is wrapped 
		int overflow;
		overflow=(buff->curr_line->width/buff->max_col);
		if(buff->curr_line->width%buff->max_col==0) overflow-=1;
		buff->row+=(overflow-(count_char_dist(buff)/buff->max_col));
		if((count_char_dist(buff)%buff->max_col)==0) buff->row-=1;
	}
	int i=0;
	buff->curr_line=(buff->curr_line)->next_line;
	buff->curr_node=(buff->curr_line)->start;
	if((buff->curr_node)->next!=NULL) { //if line is not empty move left until it reaches same position of last line
		while((buff->curr_node)->next->next!=NULL && i<buff->col) {
			if(buff->curr_node->ch=='\t') i+=8;
			else i++;
			(buff->curr_node)=(buff->curr_node)->next;
		}
	}
	buff->row+=1; // move row coordinate up 1 + how many times the line wrapped
	if(i<buff->col) buff->col=i; //if new column after moving is < previous column, update column
	move(buff->row, buff->col);
}

void mv_right(buffer *buff) {
	if((buff->curr_node)->ch=='\n') return; // if current line is empty
	if((buff->curr_node)->next->next==NULL) return; // if current node is at the end, do nothing
	if(buff->col==(buff->max_col-1)) { //if at the right edge of display
		buff->row+=1;
		buff->col=0;
	}
	else if(buff->curr_node->ch=='\t') buff->col+=8;
	else buff->col+=1;
	buff->curr_node=(buff->curr_node)->next;
	move(buff->row, buff->col);
}

void mv_left(buffer *buff) {
	if((buff->curr_node)->prev==NULL) return; // if current node is at the end, do nothing
	buff->curr_node=(buff->curr_node)->prev;
	if(buff->col==0) { //if at the left edge of display
		buff->row-=1;
		buff->col=(buff->max_col)-1;
	}
	else if(buff->curr_node->ch=='\t') buff->col-=8; //moves to closes 8th coordinate
	else buff->col-=1;
	move(buff->row, buff->col);
}
