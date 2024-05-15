
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

void selectLine_mode(buffer *buff) {
	unsigned int ch;
	int direction=0; //if + head->tail, if - tail<-head, if 0 head==tail
	line *head, *tail;
	head=buff->curr_line;
	while((ch=getch())!=ESCAPE) {
		switch(ch) {
			case UP: KEY_UP:
				 mv_up(buff);
				 direction--;
				 break;
			case DOWN: KEY_DOWN:
				   mv_down(buff);
				   direction++;
			case 'S': return;
			case COPY:
				clear_copyLine(buff);
				if(direction==0) {

				}
				if(direction>0) {

				}
				if(direction<0) {

				}
		}
	}

}

void paste(buffer *buff) {
	if(buff->copy_flag==0) return;
	if(buff->copy_flag==1) {
		dNode *curr=buff->copy_head, *save=buff->curr_node;
		int saved_col=buff->col, saved_row=buff->row;
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
}
