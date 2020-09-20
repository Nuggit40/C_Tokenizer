#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include <string.h>


void processDecimalInt(token* tok, char* input){
	char* c = input + tok->startIndex;
	while(isdigit(*c)){
		++tok->endIndex;
		++c;
	}
}

void processOctalInt(token* tok, char* input){
	char* c = input + tok->startIndex;
	while(isdigit(*c)){
		if(*c == '8' || *c == '9'){
			//switching type to decimal
			tok->endIndex = tok->startIndex;
			tok->type = decimalInt;
			processDecimalInt(tok, input);
			return;
		}
		++tok->endIndex;
		++c;
	}
}

void processWord(token* tok, char* input){
	char* c = input + tok->startIndex;
	while(isalnum(*c)){
		++tok->endIndex;
		++c;
	}
}

void processInitialChar(token* tok, char* input) {
	char c = input[tok->startIndex];
	if(isalpha(c)){
		tok->type = word;
		return;
	} else if(c == '0' && input[1] != 'x' && input[1] != 'X'){
		tok->type = octalInt;
		return;
	} else if(isdigit(c) && c != '0'){
		tok->type = decimalInt;
		return;
	} else if(c == '0' && (input[1] == 'x' || input[1] == 'X')){
		tok->type = hexadecimalInt;
		return;
	} else{
		tok->type = op;
		return;
	}
}

char* getTypeString(token* tok){
	switch(tok->type){
		case word:
			return "word";
			break;
		case decimalInt:
			return "decimal integer";
			break;
		case octalInt:
			return "octal integer";
			break;
		case hexadecimalInt:
			return "hex integer";
			break;
		case floatInt:
			return "float";
			break;
		case op:
			return "";
			break;
		default: "";
	}
}

void processToken(token* curToken, char* input, int startIndex){
	curToken->startIndex = startIndex;
	curToken->endIndex = startIndex;
	//PROCESS FIRST CHARACTER, ASSUME TYPE
	processInitialChar(curToken, input);
	//CONTUNUE READING, CHANGE TYPE IF NEEDED, UNTIL YOU RUN INTO A CHARACTER THAT IS INCOMPATABLE WITH curToken->type
	if(curToken->type == word){
		processWord(curToken, input);
	} else if(curToken->type == decimalInt){
		processDecimalInt(curToken, input);
	} else if(curToken->type == octalInt){
		processOctalInt(curToken, input);
	}
	
}

void printTokens(char* input){
	int i = 0;
	token* curToken = (token*)malloc(sizeof(token));
	if(curToken == NULL){
		printf("Memory allocation error");
	}
	while(input[i] != '\0'){
		//skips whitespace
		if(isspace(input[i])){
			++i;
			continue;
		}
		//main token processing
		processToken(curToken, input, i);
		int tokenLength = curToken->endIndex - curToken->startIndex;

		//printf("tokenlength: %d\n", tokenLength);
		//copy contents of the token in the input and prints
		char* str = (char*)malloc(sizeof(char)*tokenLength + 1);
		memcpy(str, &input[curToken->startIndex], tokenLength);
		str[tokenLength] = '\0';
		char* type_str = getTypeString(curToken);
		printf("%s \"%s\"\n", type_str, str);
		free(str);
		//printf("curtoken endindex: %d\n", curToken->endIndex);
		i = curToken->endIndex;
	}
	free(curToken);
}
int main(int argc, char* argv[]){
	printTokens("Hello World 1 0 08");
	// if (argc == 2){
	// 	printTokens(argv[1]);
	// } else {
	// 	printf("Invalid argument count\n");
	// }
}
