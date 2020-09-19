#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include <string.h>
void printTokens(char* input){
	int i = 0;
	while(input[i] != '\0'){
		//Create a token struct
		// determine type of token
		// interrogate input futher to find where the token ends given the token type
		// print out the token and advance i to the index where the last token ended
		++i;
	}
}
int main(int argc, char* argv[]){
	if (argc == 2){
		printTokens(argv[1]);
	} else {
		printf("Invalid argument count\n");
	}
}
