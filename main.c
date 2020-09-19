#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include <string.h>

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
	//PROCESS FIRST CHARACTER, ASSUME TYPE, INCREMENT endIndex IF SUCCESSFUL

	//CONTUNUE READING, CHANGE TYPE IF NEEDED, UNTIL YOU RUN INTO A CHARACTER THAT IS INCOMPATABLE WITH curToken->type

	//testing, works for words	
	curToken->type = word;
	char* c = input + startIndex;
	while(isalpha(*c)){
		++curToken->endIndex;
		++c;
	}
	
}

void printTokens(char* input){
	int i = 0;
	token* curToken = (token*)malloc(sizeof(token));
	if(curToken == NULL){
		printf("Memory allocation error");
	}
	while(input[i] != '\0'){
		processToken(curToken, input, i);
		int tokenLength = curToken->endIndex - curToken->startIndex;
		//printf("tokenlength: %d\n", tokenLength);
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
	if (argc == 2){
		printTokens(argv[1]);
	} else {
		printf("Invalid argument count\n");
	}
}
