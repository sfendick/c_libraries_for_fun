// File:	parser.c
// Author:	Tom Shehan
// Description:	A prefix calculator supporting addition, multiplication, and sqare root over ONLY INTEGERS	
// Note:	I know this is quite bad. Don't hold it against me. 
// 		Do not use floating point values, or values whose results will be floating point until it is finished.

#include "linked_list.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_TOKEN_LENGTH 100

char * compute(char * operator, char * args[]){
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	if(!strcmp(operator,"+")){
		sprintf(result,"%d",atol(args[0]) + atol(args[1]));
	}
	if(!strcmp(operator,"*")){
		sprintf(result,"%d",atol(args[0]) * atol(args[1]));
	}
	if(!strcmp(operator,"sqrt")){
		sprintf(result,"%d",(int)sqrt(atol(args[0])));
	}
	return result ;
}
int num_parameters(char * token){
	printf("\nTOKEN: %s\n",token);
	if(!strcmp(token,"+"))
		return 2;
	if(!strcmp(token,"*"))
		return 2;
	if(!strcmp(token,"sqrt"))
		return 1;
	return -1;	

}


void parse(FILE * f){
	int c ;

	char token[100];
	int token_length = 0;

	linked_list * tokens = linked_list_new();
	linked_list * num_args = linked_list_new(); 
	linked_list * required_args = linked_list_new(); 

	while((c = fgetc(f)) != EOF){
		if(c == ' ' || c == '\n' || c == '\t' || c == '(' || c == ')'){
			token[token_length] = '\0';
			if(token_length > 0){
				// push the token onto the token stack
				linked_list_push(tokens, strcpy((char *) malloc(sizeof(token)),token));

				// if the token is an operator push its argument count onto the argument requirement stack
				if(num_parameters(token) >= 0){
					printf("found a operator");
					int * num_required = (int *) malloc(sizeof(int));
					int * num_actual = (int *) malloc(sizeof(int));
					*num_required = num_parameters(token);
					*num_actual = 0;
					linked_list_push(required_args, num_required);
					linked_list_push(num_args,num_actual);
				}


				// if the token is an argument, increment the argument count at the top of the stack
				else{
					printf("found an argument");
					++(*((int*)linked_list_top(num_args)));	
				}
					
				// if the required argument count is reached, pop the arguments off their stack, pop the argument count off its stack, and replace the operator token with the result of evaluation. then check whether the argument count is reached again... this should either be recursive or (while argcount ....)
				while(*((int *)linked_list_top(num_args)) == *((int *)linked_list_top(required_args))){
					int arg_count;
					arg_count = *((int *)linked_list_top(required_args));
					char * args[arg_count];
					int i;
					for(i = 0; i < arg_count ; i++){
						args[i] = linked_list_pop(tokens);
					}
					char * operator = linked_list_pop(tokens);
					linked_list_push(tokens,compute(operator,args));

					free(linked_list_pop(num_args));
					free(linked_list_pop(required_args));
					free(operator);
					for(i = 0; i < arg_count ; i++ ){
						free(args[i]);
					}
					// if the last thing has been popped off just break out
					if(linked_list_top(num_args) == NULL){
						break;
					}

					++(*((int *)linked_list_top(num_args)));
				}
				
			}
			token_length = 0 ;
		}else{
			token[token_length] = (char) c;	
			++token_length;
		}
	}
	printf("%s",linked_list_top(tokens));

}


int main(int argc,  char * argv[]){
	FILE * f = fopen(argv[1],"r"); 
	parse(f);
	return(0);
}
