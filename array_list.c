// File:	array_list.c
// Author:	Tom Shehan
// Description:	A simple array list (vector) implementation with no memory management of contained items

#include <stdlib.h>
#include <stdio.h>
#define DEFAULT_SIZE 2

typedef struct{
	void ** items;
	int size;
	int count;
}array_list ;	

// "Constructor"
// allocates space and creates an empty array list
array_list * array_list_new(){
	array_list * l = (array_list*) malloc(sizeof(array_list));
	l->items = (void **) malloc(sizeof(void*)*DEFAULT_SIZE);
	l->size = DEFAULT_SIZE;
	l->count = 0;	
	return l;
}

// "Destructor"
// frees the array of pointers, but not the pointers themselves 
void array_list_delete(array_list * l){
	free(l->items);
	free(l);
}

// Append
// adds the item to the end of the array if there's room. 
// otherwise doubles the size, copies the values, and then adds
void array_list_append(array_list * l, void * item){
	if(l->size > l->count){
		l->items[l->count] = item;
		++(l->count);
	}else{
		l->size *= 2;
		void ** new_items = malloc(sizeof(void*) * (l->size));
		int i;
		for(i = 0; i < l->count ; i++){
			new_items[i] = l->items[i];
		}
		free(l->items);
		l->items = new_items;

		l->items[l->count] = item;
		++(l->count);
	}
}

// Remove
// empties the given index in the array. might be an unnecessary function
void array_list_remove(array_list * l, int n){
	l->items[n] = NULL;	
}

// Get
// returns the item at position n 
void * array_list_get(array_list * l, int n){
	return l->items[n];	
}

// Replace
// replaces the item at position n
void array_list_replace(array_list * l, int n,void * i){
	l->items[n] = i;	
}

// Size
// returns the number of items
int array_list_size(array_list * l){
	return l->count ;
}

/*
// Pseudo unit testing
int main(){

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 10;
	array_list * l = array_list_new();

	// Does append increase size by one?
	array_list_append(l,&a);
	array_list_append(l,&b);
	array_list_append(l,&c);
	array_list_append(l,&d);
	printf("size: %d\n",array_list_size(l));		

	// Does append actually append?
	printf("a: %d\n", *((int *)array_list_get(l,0)));
	printf("b: %d\n", *((int *)array_list_get(l,1)));
	printf("c: %d\n", *((int *)array_list_get(l,2)));
	printf("d: %d\n", *((int *)array_list_get(l,3)));

	
	// Does replace actually replace?
	array_list_replace(l,2,&e);
	printf("a: %d\n", *((int *)array_list_get(l,0)));
	printf("b: %d\n", *((int *)array_list_get(l,1)));
	printf("c: %d\n", *((int *)array_list_get(l,2)));
	printf("d: %d\n", *((int *)array_list_get(l,3)));

	
	// check valgrind to see about memory leaks
	array_list_delete(l);
}
*/
