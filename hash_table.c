// File:	hash_table.c
// Author:	Tom Shehan
// Description:	A simple hash table implementation. There is lots of testing and bug fixing needed still.
//		The "next prime" generator needs to be rewritten, and the resize needs to be tested more
//		throroughly. Specifically, I am not convinced that there is not a case which will never
//		terminate if the hashes are really unlucky. Also needs to be Valgrinded, 
//		its probably full of errors

#include <stdlib.h>
#include <stdio.h>
#define MULTIPLY_PRIME	178093613
#define DEFAULT_SIZE	11
#define MAX_CHAIN	1

typedef struct element element;
struct element{

	void * item;
	char * key ; 
	element * next;
};
typedef struct{

	element ** buckets;
	int size;
	int count;

}hash_table;



hash_table * hash_table_new(){

	hash_table * l = (hash_table *) malloc(sizeof(hash_table));
	l->buckets = (element **) calloc(DEFAULT_SIZE,sizeof(element *));
	l->size = DEFAULT_SIZE;
	l->count = 0 ;
	
}

int hash(hash_table * t,char * key){
	

	char c;
	unsigned long n = 0;

	int i = 0;
	while((c = key[i]) != '\0'){
		n += key[i];
		i++;	
	}	
	
	n *= MULTIPLY_PRIME ;
	return (n % t->size);

}

//TODO: write this for real. This is just to get it working temporarily 
// this function returns a prime greater than or equal to n,
// for use in resizing the table.each is the next prime at
// least twice as great as the previous
int get_prime(int n){
	if(n <= 11){
		return 11;	
	}
	if(n <= 23){
		return 23;	
	}
	if(n <= 47){
		return 47;	
	}
	if(n <= 97){
		return 97;	
	}
	if(n <= 197){
		return 197;	
	}
	if(n <= 397){
		return 397;	
	}
	if(n <= 797){
		return 797;	
	}
	if(n <= 1597){
		return 1597;	
	}
	if(n <= 3203){
		return 3203;	
	}
	return -1;// this will cause some kind of error and stop the program, hopefully
}

void hash_table_resize(hash_table * t){

	printf("performing resize\n");

	// find the lowest prime greater than 2 * size
	int old_size = t->size;
	t->size = get_prime(t->size * 2);
	element ** new_buckets = (element **) calloc(t->size,sizeof(element *));
	
	int i;
	for(i = 0; i < old_size ; i++){
		element * e = t->buckets[i];
		int h;
		while(e != NULL){
			element * next = e->next;
			e->next = NULL;
			h = hash(t,e->key); 
			new_buckets[h] = e;
			e = next;

		}
	}

	free(t->buckets);
	t->buckets = new_buckets;

}

void hash_table_set(hash_table * t, char * key, void * item){

	int h = hash(t,key);
	element * e = (element *) malloc(sizeof(element));
	e->item = item;
	e->key = key;
	e->next = t->buckets[h];
	t->buckets[h] = e;
	++(t->count);

	//see how deep the chain goes
	int chain = 0;
	while(e != NULL){
		e = e->next;
		chain++;
	}

	if(chain > MAX_CHAIN ){
		hash_table_resize(t);
	}

}

void * hash_table_get(hash_table * t, char  * key ){

	int h = hash(t,key);
	element * e = t->buckets[h];
	while(e != NULL){
		if(!strcmp(key,e->key)){
			return e->item ;
		}else{
			printf("going down the chain\n");
			e = e->next;
		}
	}
	return NULL;

}

// Pseduo Unit Tests
int main(){
	hash_table * t = hash_table_new();

	// Does the hash function work correctly?
	printf("Testing Hash Function:\n");
	printf("----------------------\n");
	char test [5]	= {1,2,3,4,'\0'}; // these two keys collide with the default constants 
	char test2 [5]	= {4,8,5,4,'\0'}; // we will use this property later
	printf("hash: %d\n",hash(t,test));
	printf("hash: %d\n",hash(t,test2));
	printf("%s\n\n",(hash(t,test) == (((1 + 2 + 3 + 4) * MULTIPLY_PRIME ) % 11 )) ? "passed" : "failed");

	// Does the set function set and the get function get when there are no collisions?
	printf("Testing Set/Get Functions(no collision):\n");
	printf("----------------------------------------\n");
	int item = 12358;
	hash_table_set(t,test,&item);	
	printf("item = %d\n",(*(int *)hash_table_get(t,test)));
	printf("%s\n\n",(hash_table_get(t,test) == &item) ? "passed" : "failed");
	
	// Does the set function set and the get function get when there is a collision?
	printf("Testing Set/Get Functions(with collision):\n");
	printf("------------------------------------------\n");
	int item2 = 55555;
	hash_table_set(t,test2,&item2);	
	printf("%d\n",(*(int *)hash_table_get(t,test))); // get the original item, which will be down the chain
	printf("%d\n",(*(int *)hash_table_get(t,test2))); // get the second item, which will be on top
	printf("%s\n\n",(hash_table_get(t,test2) == &item2) ? "passed" : "failed");

	// Has the hash table been resized?
	printf("Testing whether the hash table has been resized:\n");
	printf("------------------------------------------\n");
	printf("hash: %d\n",hash(t,test));
	printf("hash: %d\n",hash(t,test2));
	printf("If so, these values should not be equal to the earlier hash values\n");
	printf("As a bonus, they will no longer collide, which is why the expansion was necessary in the first place\n\n");

	// Does get return NULL if nothing set?
	printf("Testing Get Failure:\n");
	printf("--------------------\n");
	printf("%x\n",hash_table_get(t,"abcdefgh"));
	printf("%s\n\n",(hash_table_get(t,"abcdefgh") == NULL) ? "passed" : "failed");



	// Some fun stuff
	hash_table * h = hash_table_new();

	char name[] = "Fake";
	hash_table_set(h,"first_name",&name);
	char last[] = "Person";
	hash_table_set(h,"last_name",&last);
	int age = 100;
	hash_table_set(h,"age",&age);

	printf("My full name is %s %s. I am %d years old\n",(char *)hash_table_get(h,"first_name"), (char *)hash_table_get(h,"last_name"),*((int*)hash_table_get(h,"age")));


}
