// File:	parser.c
// Author:	Tom Shehan
// Description:	A prefix calculator supporting addition, multiplication, and sqare root over ONLY INTEGERS	
// Note:	Although the program is still sloppy, it is much improved fundamentally.
//
// TODO:	see if conditional statements can be simplified to a state machine
// 		fix newly introduced memory leaks
//		make the error detection cleaner
//		review for potenntial errors

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
		long a = strtol(args[0],&end,10);
		long b = strtol(args[1],&end,10);
		sprintf(result,"%d",a + b);
	}else{
		double a = strtod(args[0],&end);
		double b = strtod(args[1],&end);
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
		long a = strtol(args[0],&end,10);
		long b = strtol(args[1],&end,10);
		sprintf(result,"%d",a - b);
	}else{
		double a = strtod(args[0],&end);
		double b = strtod(args[1],&end);
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
		long a = strtol(args[0],&end,10);
		long b = strtol(args[1],&end,10);
		sprintf(result,"%d",a * b);
	}else{
		double a = strtod(args[0],&end);
		double b = strtod(args[1],&end);
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
		long a = strtol(args[0],&end,10);
		long b = strtol(args[1],&end,10);
		sprintf(result,"%d",a / b);
	}else{
		double a = strtod(args[0],&end);
		double b = strtod(args[1],&end);
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

char * power(char * args[]){
	char * end;
	char * result = (char *) malloc(MAX_TOKEN_LENGTH * sizeof(char));
	double a = strtod(args[0],&end);
	double b = strtod(args[1],&end);
	sprintf(result,"%f",pow(a,b));
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
	if(!strcmp(operator,"pow")){
		return power(args);
	}
}

int required_arguments(char * operator){

	if(!strcmp(operator,"+")){
		return 2;
	}
	if(!strcmp(operator,"-")){
		return 2;
	}
	if(!strcmp(operator,"*")){
		return 2;
	}
	if(!strcmp(operator,"/")){
		return 2;
	}
	if(!strcmp(operator,"sqrt")){
		return 1;
	}
	if(!strcmp(operator,"pow")){
		return 2;
	}

}

char * parse(FILE  * f){

	int	c ;				// current scanned character
	char	token[MAX_TOKEN_LENGTH];	// current token being built
	int	token_length = 0;		// index in the token for next character
	char *	error = (char*)malloc(MAX_TOKEN_LENGTH);	// error message
		
	linked_list * tokens = linked_list_new();	// the entire stack of tokens
	linked_list * arguments = linked_list_new();	// the arguments for a single operation being executed

	while((c = fgetc(f)) != EOF){	

		// if the current character is not whitespace, or a parenthesis, add it to the current token
		if(c != ' ' && c != '\n' && c != '\t' && c != '(' && c != ')'){
			token[token_length] = c;
			++token_length;

		// otherwise, push the token on the stack as-is. We will deal with the current character in a moment 
		}else{

			// push only if not an empty token
			if(token_length > 0){
				token[token_length] = '\0';
				char * token_copy = (char *) malloc(token_length + 1);
				strcpy(token_copy,token);
				linked_list_push(tokens,token_copy);	
				token_length = 0 ;
			}

			// if the current character is an opening parenthesis, push it on the token stack
			if(c == '('){
				char * new_token = (char *) malloc(2);
				new_token[0] = c;
				new_token[1] = '\0';
				linked_list_push(tokens,new_token);

			// if the current character is a closing parenthesis, dont bother pushing it on the stack, just evaluate
			}else if(c == ')'){
				

					// catch extra closing parenthesis error
					if(linked_list_top(tokens)==NULL){
						sprintf(error,"Error: extra closing parenthesis");
						return(error);
					}	

					// put the arguments in their own stack and count them. stop when a opening parenthesis is reached
					int arg_count = -1 ;
					while(*((char *) linked_list_top(tokens)) != '('){
						linked_list_push(arguments,linked_list_pop(tokens));	
						++arg_count;
					}

					// catch empty parenthesis
					if(arg_count == -1){
						sprintf(error,"Error: No function name supplied.");
						return(error);
					}

					char * operator = (char *) linked_list_pop(arguments); // the top "argument" is actually the operator

					// catch too few or too many arguments
					if(arg_count <  required_arguments(operator)){
						sprintf(error,"Error: Not enough arguments for %s", operator);
						return(error);
					}else if(arg_count > required_arguments(operator)){
						sprintf(error,"Error: Too many arguments for %s", operator);
						return(error);
					}


					free(linked_list_pop(tokens));	// free the closing parenthesis

					// transfer the arguments to an array to get them in the correct order	
					char * arg_array[arg_count];
					int i = 0;
					while(i < arg_count){
						arg_array[i] = linked_list_pop(arguments) ;
						++i;
					}


					// perform the actual operation and push the result onto the token stack
					linked_list_push(tokens,compute(operator, arg_array)); 


					// free everything for this expression
					free(operator);				// the operator
					for(i = 0 ; i < arg_count ; i++){	// the arguments
						free(arg_array[i]);
					}
				
			}
		}


	}

	// catch no equation  error
	if(linked_list_size(tokens) == 0){		
		sprintf(error,"Error: No equation entered");
		return(error);
	}

	// catch extra opening parenthesis error
	if(*((char*)linked_list_top(tokens)) == '(' || linked_list_size(tokens) > 1){		
		sprintf(error,"Error: Extra opening parenthesis");
		return(error);
	}

	char * result = linked_list_top(tokens);	
	free(tokens);
	return result ;
}	


int main(int argc,  char * argv[]){

	if(argc > 1){
		FILE * f = fopen(argv[1],"r"); 
		printf("%s\n",parse(f));
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
				input[i]= '\n';
				input[i+1]= '\0';
				FILE * f = fopen(".calc_temp","w");
				fputs(input,f);
				fclose(f);
				f = fopen(".calc_temp","r");
				printf("%s\n",parse(f));
				i = 0;
			}
		}
	}
	
	return(0);
}
