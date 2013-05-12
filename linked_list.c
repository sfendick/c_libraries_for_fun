// File:	linked_list.c
// Author:	Tom Shehan
// Description:	A simple linked list implementation with no memory management of contained items

#include <stdlib.h>
#include <stdio.h>

// The recursive linked list structure.
// Contains an item and a link
typedef struct linked_list  linked_list;
struct linked_list{
	void * item ;
	linked_list * next ;
};

// "Constructor"
// allocates space and creates an empty list
linked_list * linked_list_new(){
	linked_list * l = (linked_list*) malloc(sizeof(linked_list));
	l->item = NULL;
	l->next = NULL; 
	return l;
}

// "Destructor"
// frees memory from every link but not its item 
void linked_list_delete(linked_list * l){

	linked_list * next = l->next;
	free(l);

	if(next != NULL){
		linked_list_delete(next);
	}

}

// Push
// adds a new link containing the item at the head of the list
void linked_list_push(linked_list * l, void * item){

	if(l->item == NULL){
		l->item = item ;
		return;
	}

	linked_list * next = l->next;	// temporarily store the next link's address
	l->next = linked_list_new();	// replace the next link with an empty link
	l->next->item = l->item;	// put the current link's item in the new empty link
	l->next->next = next;		// put the stored next link address in the new link's next field
	l->item = item;			// put the new item in the current link
}

// Pop
// removes the link at the head of the list and returns its item
void * linked_list_pop(linked_list * l){
	void * item = l->item;
	if(l->next != NULL){
		l->item = l->next->item;
		linked_list * next = l->next;
		l->next = l->next->next;
		free(next);
	}else{
		l->item = NULL;
	}
	return(item);
}

// Top
// returns the item in the link at the head of the list
void * linked_list_top(linked_list * l){
	return l->item;
}

// Next
// returns the next link in the list. Used as an iterator
linked_list * linked_list_next(linked_list * l){
	return l->next;
}

// Size
// returns the number of non-empty links in the list
int linked_list_size(linked_list * l){
	if(l->item == NULL){
		return(0);
	}
	if(l->next == NULL){
		return(1);
	}
	return(1 + linked_list_size(l->next));
}

/*
// Pseudo Unit Testing
int main(){
	
	int test_item = 10;
	int top_item = 20;

	// Is there a memory leak when deleting an entire list? Valgrind will let me know
	linked_list * l = linked_list_new();
	linked_list_push(l,&test_item);
	linked_list_push(l,&test_item);
	linked_list_push(l,&test_item);
	linked_list_push(l,&test_item);
	linked_list_delete(l);

	// Is there a memory leak when pushing/popping(and then deleting an empty list)? Valgrind will let me know
	l = linked_list_new();
	linked_list_push(l,&test_item);
	linked_list_push(l,&test_item);
	linked_list_push(l,&test_item);
	linked_list_pop(l);
	linked_list_pop(l);
	linked_list_pop(l);
	linked_list_delete(l);

	// Does push increase the size by one?
	l = linked_list_new();
	linked_list_push(l,&test_item);	
	linked_list_push(l,&test_item);	
	linked_list_push(l,&test_item);	
	if(linked_list_size(l) == 3){
		printf("passed");
	}else{
		printf("failed");
	}

	// Does top get the top item?
	linked_list_push(l,&top_item);	
	if(*((int *)linked_list_top(l)) == 20){
		printf("passed");
	}else{
		printf("failed");
	}
	
	// Does pop get the top item?
	linked_list_push(l,&top_item);	
	if(*((int *)linked_list_pop(l)) == 20){
		printf("passed");
	}else{
		printf("failed");
	}


	// Does pop decrease the size by one?
	linked_list_pop(l);
	linked_list_pop(l);
	linked_list_pop(l);
	linked_list_pop(l);
	if(linked_list_size(l) == 0){
		printf("passed");
	}else{
		printf("failed");
	}

	// Does next allow iteration through the whole list?
	linked_list_delete(l);
	l = linked_list_new();
	linked_list_push(l,&test_item);	
	linked_list_push(l,&test_item);	
	linked_list_push(l,&test_item);	
	linked_list * i;
	int j = 0;
	for(i = l; i != NULL ; i = linked_list_next(i)){
		j++;
	}
	if(j == 3){
		printf("passed");
	}else{
		printf("failed");
	}

	linked_list_delete(l);
	return(0);
}
*/
