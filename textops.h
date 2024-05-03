#define ESCAPE '\e'
#define BACKSPACE '\b'
#define NEWLINE '\n'

void display_insert(dNode *curr_node, char input) {
	addch(input);
	if(curr_node->ch=='\0') return;
	while(curr_node!=NULL) {
		addch(curr_node->ch);
		curr_node=curr_node->next;
	}
	refresh();
}

void insert_to_list(dNode **curr_node, line *curr_line, char input) {
	dNode *new_node=malloc(sizeof(dNode)); 
	curr_line->size+=1;
	if(curr_line->start==NULL) { // if empty line
		curr_line->start=new_node;
		new_node->prev=NULL;
		(*curr_node)=malloc(sizeof(dNode));
		(*curr_node)->ch='\0';
		(*curr_node)->next=NULL;
	}
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

void insert_mode(int *row, int *col, line **curr_line, dNode **curr_node) {
	char key;
	while((key=getch())!=ESCAPE) {
		switch(key) {
			case BACKSPACE:
				break;
			case NEWLINE:
				break;
			default:
				insert_to_list(curr_node, *curr_line, key);
				display_insert(*curr_node, key);
				move(*row, ++(*col));
		}
	}
	if((*curr_node)->ch=='\0') {
		mv_left(row, col, curr_line, curr_node);
		free((*curr_node)->next);
		(*curr_node)->next=NULL;
	}
}
