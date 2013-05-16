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

// Split
// breaks off the list starting with position n and returns the new list
linked_list * linked_list_split(linked_list * l,int n){

	linked_list * current = l;
	linked_list * next;

	// if n is zero, the new list will contain everything and the existing list will be empty
	if(n == 0){
		current = malloc(sizeof(linked_list));
		current->item = l->item;
		current->next = l->next;
		l->item = NULL;
		l->next = NULL;
		return current;	
	}


	// Iterate to the n'th element and split the list there.
	// this requires setting the n-1th link's next field to null and returning the nth link
	int i ;
	for(i = 0; i < n ; i++){

		// if the next item is moving to the other list, remove the link to it
		next = current->next;
		if(i + 1 == n){
			current->next = NULL;
		}

		// if we reach the end of the list before n, the split returns an empty list
		// and the original list remains unchanged
		if(current == NULL){
			return linked_list_new();
		}

		current = next;
	}

	return current;	

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

// Next
// returns the next link in the list. Used as an iterator
linked_list * linked_list_next(linked_list * l){
	return l->next;
}

// Merge
// merges b into a and frees b
linked_list_merge(linked_list * a, linked_list * b, int (*compare)(void *, void *)){

	// If either or both lists are empty, no action is taken
	if(linked_list_top(a) == NULL || linked_list_top(b) == NULL){
		return;
	}

	// Iterate over every link in a, prepending with elements from b if they are "higher ranked"
	linked_list * i;
	for(i = a; i != NULL; i = linked_list_next(i)){

		// If list B runs out first, we are done
		if(linked_list_top(b) == NULL){
			free(b);
			return;
		}
		
		// if the element from B is "higher ranked", pop it off B and push it on A
		if(compare(linked_list_top(i),linked_list_top(b)) >= 0){
			linked_list_push(i,linked_list_pop(b));
		}

		// if list A runs out first, append the rest of B
		if(linked_list_next(i) == NULL){
			i->next = b;
			return;
		}

	}

}

// Sort
// perform mergesort on the list
void linked_list_sort(linked_list * l, int (*compare)(void *, void *)){

	int size = linked_list_size(l);
	
	// base case
	if(size == 1 || size == 0){
		return;
	}

	// recursive case
	int split = size / 2;
	linked_list * b = linked_list_split(l,split);
	linked_list_sort(l,compare);
	linked_list_sort(b,compare);
	linked_list_merge(l,b,compare);
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


int my_compare(void * a, void * b){
	if( *((int*)a) > *((int*)b) ){
		return -1;
	}else if( *((int*)a) < *((int*)b) ){
		return 1;
	}else{
		return 0;
	}
}
int str_compare(void * a, void * b){
	return strcmp((char *)a,(char *)b);
}


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



	// Test sort with integers
	linked_list * l = linked_list_new();
	int a = 7;
	linked_list_push(l,&a);
	int b = 3;
	linked_list_push(l,&b);
	int c = 9;
	linked_list_push(l,&c);
	int d = 1;
	linked_list_push(l,&d);
	int e = 5;
	linked_list_push(l,&e);
	int f = 2;
	linked_list_push(l,&f);
	int g = 6;
	linked_list_push(l,&g);
	int h = 8;
	linked_list_push(l,&h);
	linked_list_sort(l,&my_compare);
	linked_list * i = l;
	while(i != NULL ){
		printf("top: %d\n" ,*((int *)linked_list_top(i)));
		i = i->next;
	}
	linked_list_delete(l);

	// test sort with strings
	linked_list * k = linked_list_new();
	char k1[] = "hello";
	char k2[] = "world";
	char k3[] = "goodbye";
	linked_list_push(k,k1);
	linked_list_push(k,k2);
	linked_list_push(k,k3);
	linked_list_sort(k,&str_compare);
	i = k;
	while(i != NULL ){
		printf("top: %s\n" , (char*)linked_list_top(i));
		i = i->next;
	}
	linked_list_delete(k);
	


	return(0);
}
*/


