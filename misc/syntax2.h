#include <ctype.h>
#define MAGENTA 2
#define YELLOW 3
#define CYAN 4
#define GREEN 5
#define WHITE 6

int is_separator(char ch) { // function for checking if char is separator
	return isspace(ch) || ch=='\n' || strchr(",.()+-/*=~%<>[];", ch)!=NULL; // strchr checks if ch is any of the chars in string given
}
void update_syntax(buffer *buff) {
	line *curr_line=buff->top;
	dNode *curr_node=curr_line->start;
	char ch;
	int in_string=0;
	char *keywords1[] = {"switch", "if", "while", "for", "break", "continue", "return", "else", "struct", "typedef", "case", "int", "long", "double", "float", "char", "unsigned", "signed", "void", "#define"}; // 12
//	char *keywords2[] = {"int", "long", "double", "float", "char", "unsigned", "signed", "void", "#define"};// 8

	while(curr_line!=NULL) { //reset
		while(curr_node!=NULL) {
			curr_node->color=WHITE;
			curr_node=curr_node->next;
		}
		curr_line=curr_line->next_line;
		if(curr_line==NULL) break;
		curr_node=curr_line->start;
	}
	curr_line=buff->top;
	curr_node=curr_line->start;
	while(curr_line!=NULL) {
		while(curr_node!=NULL) { 
			ch=curr_node->ch;
			if(curr_node->prev==NULL) { // for digits
				if(isdigit(ch)) {
					curr_node->color=MAGENTA;
				}
					int j;
					for(j=0; j<20; j++){
						int slen=strlen(keywords1[j]), count=0;
						dNode *search=curr_node, *started=curr_node;
						while(search->next!=NULL && search->ch==keywords1[j][count]) {
							search=search->next;
							count++;
						}
						if(keywords1[j][count]=='\0') {
							if(search->next==NULL) {
								while(started!=search) {
									started->color=YELLOW;
									started=started->next;
								} started->color=YELLOW;
								break;
							}
							else if(is_separator(search->ch)) {
								while(started!=search) {
									started->color=YELLOW;
									started=started->next;
								} started->color=YELLOW;
								break;
							}
						}
					}	
			}
			else if(isdigit(ch) && (curr_node->prev->color==MAGENTA || is_separator(curr_node->prev->ch))) { //if current char is a digit
				curr_node->color=MAGENTA;
			}
			else if(in_string) { //for strings
				curr_node->color=GREEN;
				if(ch==in_string) in_string=0;
			}
			else if(ch=='"'||ch=='\'') {
				in_string=ch;
				curr_node->color=GREEN;
			}
			else if(curr_node->prev!=NULL) {
				if(is_separator(curr_node->prev->ch)) {
				int j;
				for(j=0; j<20; j++){
					int slen=strlen(keywords1[j]), count=0;
					dNode *search=curr_node, *started=curr_node;
					while(search->next!=NULL && search->ch==keywords1[j][count]) {
						search=search->next;
						count++;
					}
					if(keywords1[j][count]=='\0') {
						if(search->next==NULL) {
							while(started!=search) {
								started->color=YELLOW;
								started=started->next;
							} started->color=YELLOW;
							break;
						}
						else if(is_separator(search->ch)) {
							while(started!=search) {
								started->color=YELLOW;
								started=started->next;
							} started->color=YELLOW;
							break;
						}
					}
				}	
				}
			}
			curr_node=curr_node->next;
		}
		curr_line=curr_line->next_line;
		if(curr_line==NULL) break;
		curr_node=curr_line->start;
	}
}
