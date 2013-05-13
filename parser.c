// File:	parser.c
// Author:	Tom Shehan
// Description:	A prefix calculator supporting addition, multiplication, and sqare root over ONLY INTEGERS	
//
// Note:	I know this is quite bad. Don't hold it against me. The parser is not theoretically sound, 
// 		and has a couple of "extra features". Also, poorly formed equations cause segfaults. These
// 		will be the next things to be changed now that floating point arithmetic works.

#include "linked_list.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_TOKEN_LENGTH 100
#define MAX_INPUT_SIZE 1000


typedef enum{
	INTEGER,
	FLOAT,
	BAD
}token_type;


token_type get_type(char * token){

	char * end;

	// see if the token is an integer
	long num_l = strtol(token,&end,10);
	if(*end == '\0'){
		return INTEGER;
	}

	// see if the token is an integer
	double num_f = strtod(token,&end);
	if(*end == '\0'){
		return FLOAT;
	}	

	return BAD;
}


char * add(char * args[]){
	token_type t0 = get_type(args[0]);
	token_type t1 = get_type(args[1]);
	char * end;
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	
	if(t0 == INTEGER && t1 == INTEGER){		
		long a = strtol(args[1],&end,10);
		long b = strtol(args[0],&end,10);
		sprintf(result,"%d",a + b);
	}else{
		double a = strtod(args[1],&end);
		double b = strtod(args[0],&end);
		sprintf(result,"%f",a + b);
	}
	return(result);
}

char * subtract(char * args[]){
	token_type t0 = get_type(args[0]);
	token_type t1 = get_type(args[1]);
	char * end;
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	
	if(t0 == INTEGER && t1 == INTEGER){		
		long a = strtol(args[1],&end,10);
		long b = strtol(args[0],&end,10);
		sprintf(result,"%d",a - b);
	}else{
		double a = strtod(args[1],&end);
		double b = strtod(args[0],&end);
		sprintf(result,"%f",a - b);
	}
	return(result);
}

char * multiply(char * args[]){
	token_type t0 = get_type(args[0]);
	token_type t1 = get_type(args[1]);
	char * end;
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	
	if(t0 == INTEGER && t1 == INTEGER){		
		long a = strtol(args[1],&end,10);
		long b = strtol(args[0],&end,10);
		sprintf(result,"%d",a * b);
	}else{
		double a = strtod(args[1],&end);
		double b = strtod(args[0],&end);
		sprintf(result,"%f",a * b);
	}
	return(result);
}

char * divide(char * args[]){
	token_type t0 = get_type(args[0]);
	token_type t1 = get_type(args[1]);
	char * end;
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	
	if(t0 == INTEGER && t1 == INTEGER){		
		long a = strtol(args[1],&end,10);
		long b = strtol(args[0],&end,10);
		sprintf(result,"%d",a / b);
	}else{
		double a = strtod(args[1],&end);
		double b = strtod(args[0],&end);
		sprintf(result,"%f",a / b);
	}
	return(result);
}

char * square_root(char * args[]){
	char * end;
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	double a = strtod(args[0],&end);
	sprintf(result,"%f",sqrtf(a));
	return(result);
}







char * compute(char * operator, char * args[]){
	if(!strcmp(operator,"+")){
		return add(args);
	}
	if(!strcmp(operator,"-")){
		return subtract(args);
	}
	if(!strcmp(operator,"*")){
		return multiply(args);
	}
	if(!strcmp(operator,"/")){
		return divide(args);
	}
	if(!strcmp(operator,"sqrt")){
		return square_root(args);
	}
}
int num_parameters(char * token){
	if(!strcmp(token,"+"))
		return 2;
	if(!strcmp(token,"-"))
		return 2;
	if(!strcmp(token,"*"))
		return 2;
	if(!strcmp(token,"/"))
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
					int * num_required = (int *) malloc(sizeof(int));
					int * num_actual = (int *) malloc(sizeof(int));
					*num_required = num_parameters(token);
					*num_actual = 0;
					linked_list_push(required_args, num_required);
					linked_list_push(num_args,num_actual);
				}


				// if the token is an argument, increment the argument count at the top of the stack
				else{
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
	printf("%s\n",linked_list_top(tokens));
	free(linked_list_pop(tokens));
	free(tokens);
	free(num_args);
	free(required_args);

}


int main(int argc,  char * argv[]){

	if(argc > 1){
		FILE * f = fopen(argv[1],"r"); 
		parse(f);
		fclose(f);
	}else{
		int c;
		char input[MAX_INPUT_SIZE + 1];
		int i = 0;
		while((c = getchar()) != EOF){
			if(c != '\n'){
				input[i] = c;	
				++i;
			}else{
				input[i]= '\0';
				FILE * f = fopen(".calc_temp","w");
				fputs(input,f);
				fclose(f);
				f = fopen(".calc_temp","r");
				parse(f);
				i = 0;
			}
		}
	}
	
	return(0);
}
