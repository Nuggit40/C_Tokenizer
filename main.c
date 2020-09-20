#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include <string.h>

void processFloatInt(token* tok, char* input){
	//HANDLE >1 period
	int periodRead = 0;
	char* c = input + tok->startIndex;
	//reading until you encounter '.' and digits that follow it
	while(isdigit(*c) || *c == '.'){
		if(periodRead && *c == '.'){
			break;
		} else if(*c == '.'){
			++periodRead;
		}
		++tok->endIndex;
		++c;
	}
	//if you run into exponent e-10 for example
	// if(*c == 'e' && *(c+1) == '-'){
	// 	++tok->endIndex;
	// 	++c;
	// 	//reading exponent digits
	// 	while(isdigit(*c)){
	// 		++tok->endIndex;
	// 		++c;
	// 	}
	// }
}

int isHexChar(char c){
	if(isdigit(c) || c == 'a' || c == 'A' || c == 'b' || c == 'B' || c == 'c' || c == 'C' || c == 'd' || c == 'D' || c == 'e' || c == 'E' || c == 'f' || c == 'F'){
		return 1;
	}
	return 0;
}

void processHexadecimalInt(token* tok, char* input){
	char* c = input + tok->startIndex + 2;
	tok->endIndex += 2;
	//assuming the first 2 chars are '0x'
	while(isHexChar(*c)){
		++tok->endIndex;
		++c;
	}
}

void processDecimalInt(token* tok, char* input){
	

	char* c = input + tok->startIndex;
	while(isdigit(*c)){
		++tok->endIndex;
		++c;
	}
	//convert to float if appropriate, (. and digit that follows)
	if(*c == '.' && isdigit(*(c+1))){
		tok->endIndex = tok->startIndex;
		tok->type = floatInt;
		processFloatInt(tok, input);
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
	char next = input[tok->startIndex + 1];
	if(isalpha(c)){
		tok->type = word;
		return;
	} else if(c == '0' && (next == 'x' || next == 'X')){
		tok->type = hexadecimalInt;
		return;
	} else if(isdigit(c) && c != '0'){
		tok->type = decimalInt;
		return;
	} else if(c == '0' && next != 'x' && next != 'X'){
		tok->type = octalInt;
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
			return "hex";
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
	switch(curToken->type){
		case word:
			processWord(curToken, input);
			break;
		case decimalInt:
			processDecimalInt(curToken, input);
			break;
		case octalInt:
			processOctalInt(curToken, input);
			break;
		case hexadecimalInt:
			processHexadecimalInt(curToken, input);
			break;
		case op:
			break;

	}
	
}

void printTokens(char* input){
	int i = 0;
	token* curToken = (token*)malloc(sizeof(token));
	if(curToken == NULL){
		printf("Memory allocation error");
		return;
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
	printTokens("3.14159 Hello World 132 0712 0xF9");
	// if (argc == 2){
	// 	printTokens(argv[1]);
	// } else {
	// 	printf("Invalid argument count\n");
	// }
}
