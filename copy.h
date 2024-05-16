
#define UP 'k'
#define DOWN 'j'
#define RIGHT 'l'
#define LEFT 'h'
#define COPY 'c'

void clear_copybuffer(buffer *buff) {
	if(buff->copy_head==NULL) return;
	dNode *temp=buff->copy_head, *q=temp;
	while(temp->next!=NULL) {
		q=temp;
		temp=temp->next;
		free(q);
	}free(temp);
}

void select_mode(buffer *buff) {
	unsigned int ch;
	int direction=0; //if + head->tail, if - tail<-head, if 0 head==tail
	dNode *head, *tail;
	head=buff->curr_node;
	while((ch=getch())!=ESCAPE) {
		switch(ch) {
			case LEFT: case KEY_LEFT:
				if(buff->curr_node->prev!=NULL) direction--;
				mv_left(buff);
				break;
			case RIGHT: case KEY_RIGHT:
				if(buff->curr_node->next->ch!='\n') direction++;
				mv_right(buff);
				break;
			case 's': //typed s again
				return;
			case COPY:
				clear_copybuffer(buff); //clear old copy for new copy
				if(direction==0) {
					buff->copy_head=malloc(sizeof(dNode));
					buff->copy_head->ch=head->ch;
					buff->copy_head->next=NULL;
					buff->copy_tail=buff->copy_head;
					return;
				}
				else if(direction>0) tail=buff->curr_node;
				else if(direction<0) {
					tail=head;
					head=buff->curr_node;
				}
				dNode *curr=malloc(sizeof(dNode));
				buff->copy_head=curr;
				curr->ch=head->ch;
				while(head!=tail) { //copy chars to copybuff
					head=head->next;
					curr->next=malloc(sizeof(dNode));
					curr=curr->next;
					curr->ch=head->ch;
				}
				buff->copy_tail=curr;
				curr->next=NULL;
				buff->copy_flag=1;
				return;	
		}
	}
}

dNode* copy_line(dNode *source) {
	dNode *curr=malloc(sizeof(dNode)), *head, *copy=source;
	head=curr;
	curr->ch=copy->ch;
	while(copy->next!=NULL) { //copy all nodes from source to curr
		copy=copy->next;
		curr->next=malloc(sizeof(dNode));
		curr=curr->next;
		curr->ch=copy->ch;
	}	
	curr->ch=copy->ch;
	curr->next=NULL;
	return head;
}
void clear_copyLine(buffer *buff) {
	if(buff->copy_line_head==NULL) return;
	line *temp_line=buff->copy_line_head, *p;
	dNode *temp, *q;
	while(temp_line->next_line!=NULL) {
		p=temp_line;
		temp_line=temp_line->next_line;
		temp=p->start;
		while(temp->next!=NULL) { //free all nodes
			q=temp;
			temp=temp->next;
			free(q);
		} free(temp);
		free(p); //free line
	}
}

void selectLine_mode(buffer *buff) {
	unsigned int ch;
	int direction=0; //if + head->tail, if - tail<-head, if 0 head==tail
	line *head, *tail;
	head=buff->curr_line;
	while((ch=getch())!=ESCAPE) {
		switch(ch) {
			case UP: case KEY_UP:
				mv_up(buff);
				direction--;
				break;
			case DOWN: case KEY_DOWN:
			  	mv_down(buff);
			   	direction++;
				break;
			case 'S': return;
			case COPY:
				clear_copyLine(buff);
				if(direction==0) {
					buff->copy_line_head=malloc(sizeof(line));
					buff->copy_line_head->start=copy_line(head->start);
					buff->copy_line_head->next_line=NULL;
					return;
				}
				if(direction>0) {
					tail=buff->curr_line;	
				}
				if(direction<0) {
					tail=head;
					head=buff->curr_line;
				}
				line *curr=malloc(sizeof(line));
				buff->copy_line_head=curr;
				curr->start=copy_line(head->start);
				while(head!=tail) {
					head=head->next_line;
					curr->next_line=malloc(sizeof(line));
					curr=curr->next_line;
					curr->start=copy_line(head->start);
				}
				curr->next_line=NULL;
				buff->copy_flag=2;
				return;
		}
	}
}

void paste(buffer *buff) {
	if(buff->copy_flag==0) return; //empty copy buffers
	if(buff->copy_flag==1) { //paste text
		dNode *curr=buff->copy_head;
		append(buff);
		while(curr!=NULL) {
			insert_to_list(buff, curr->ch);
			if(buff->col==buff->max_col-1) { buff->col=0; buff->row+=1; }
			else buff->col+=1;
			curr=curr->next;
		}
		display(buff);
		move(buff->row, buff->col);
	}
	if(buff->copy_flag==2) { //paste line
		line *curr=buff->copy_line_head, *save_line=buff->curr_line->next_line, *new_line=malloc(sizeof(line));
		int save_row=buff->row, save_col=buff->col;
		buff->curr_line->next_line=new_line;
		new_line->prev_line=buff->curr_line->prev_line;
		new_line->start=copy_line(curr->start);
		while(curr->next_line!=NULL) {
			curr=curr->next_line;
			new_line->next_line=malloc(sizeof(line));
			new_line->next_line->prev_line=new_line;
			new_line=new_line->next_line;
			new_line->start=copy_line(curr->start);
		}
		new_line->next_line=save_line;
		buff->row=save_row;
		buff->col=save_col;
		display(buff);
		move(buff->row, buff->col);
	}
}
