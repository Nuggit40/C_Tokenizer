#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include <string.h>
//Finds the type and size of an operator and checks for unsupported operators
void processOperator(token* tok, char* input){
	char* c = input + tok->startIndex;
	char next = input[tok->startIndex + 1];
	char next2 = input[tok->startIndex + 2];
	//assuming token is at least 1 length
	++tok->endIndex;
	//checking for unsupported characters
	if(*c == '\\' || *c == '\"' || *c == '`'){
		tok->opType = unknown;
	};
	if (*c == '+'){
		if (next == '+'){
			tok->opType = increment;
			++tok->endIndex;
		}
		else if (next == '='){
			tok->opType = plus_equals;
			++tok->endIndex;
		}
		else {
			tok->opType = addition;
		}	
	}
	else if(*c == '<'){
		if (next == '<'){
			++tok->endIndex;
			if (next2 == '='){
				tok->opType = shift_left_equals;
				++tok->endIndex;
			}		
			else {
				tok->opType = shift_left;
			}
		}
		else if(next == '='){
			++tok->endIndex;
			tok->opType = less_than_or_equal_test;
		}
		else {
			tok->opType = less_than_test;
		}

	}
	else if(*c == '>'){
		if (next == '>'){
			++tok->endIndex;
			if (next2 == '='){
				tok->opType = shift_right_equals;
				++tok->endIndex;
			}
			else{
				tok->opType = shift_right;
			}
		}
		else if (next == '='){
			tok->opType = greater_than_or_equal_test;
			++tok->endIndex;
		}
		else{
			tok->opType = greater_than_test;
		}

	}
	else if(*c == '-'){
		if (next== '-'){
			tok->opType = decrement;
			++tok->endIndex;	
		}
		else if(next == '='){
			tok->opType = minus_equals;
			++tok->endIndex;
		}
		else if(next == '>'){
			tok->opType = structure_pointer;
			++tok->endIndex;
		}
		else{
			tok->opType = minus_subtract_operator;
		}
	}
	else if(*c == '('){
		tok->opType = left_parenthesis;
	}
	else if (*c == ')'){
		tok->opType = right_parenthesis;
	}
	else if (*c == '['){
		tok->opType = left_bracket;
	}
	else if (*c == ']'){
		tok->opType = right_bracket;
	}
	else if(*c == '.'){
		tok->opType = structure_member;

	}
	else if(*c == ','){
		tok->opType = comma;
	}
	else if(*c == '!'){
		if(next == '='){
			tok->opType = inequality_test;
			++tok->endIndex;
		}
		else{
			tok->opType = negate;
		}
	}
	else if(*c == '~'){
		tok->opType = ones_complement;
	}
	else if(*c == '^'){
		if(next == '='){
			tok->opType = bitwise_XOR_equals;
			++tok->endIndex;
		}
		else{
			tok->opType = bitwise_XOR;
		}
	}
	else if(*c == '|'){
		if(next == '|'){
			tok->opType = logical_OR;
			++tok->endIndex;
		}
		else if(next == '='){
			tok->opType = bitwise_OR_equals;
			++tok->endIndex;
		}
		else{
			tok->opType = bitwise_OR;
		}
	}
	else if(*c == '/'){
		if(next == '='){
			tok->opType = divide_equals;
			++tok->endIndex;
		}
		else{
			tok->opType = division;
		}
	}
	else if(*c == '&'){
		if(next == '&'){
			tok->opType = logical_AND;
			++tok->endIndex;
		}
		else if(next == '='){
			tok->opType = bitwise_AND_equals;
			++tok->endIndex;
		}
		else{
			tok->opType = AND_address_operator;
		}
	}
	else if(*c == '?'){
		tok->opType = conditional_true;
	}
	else if(*c == ':'){
		tok->opType = conditional_false;
	}
	else if(*c == '='){
		if(next == '='){
			tok->opType = equality_test;
			++tok->endIndex;
		}
		else{
			tok->opType = assignment;
		}
	}
	else if(*c == '*'){
		if(next == '='){
			tok->opType = times_equals;
			++tok->endIndex;
		}
		else{
			tok->opType = multiply_dereference_operator;
		}
	}
	else if(*c == '%'){
		if(next == '='){
			tok->opType = mod_equals;
			++tok->endIndex;
		} else {
			tok->opType = modulus;
		}
	}
}
//Finds out how many characters a float token is
void processFloat(token* tok, char* input){
	int periodRead = 0;
	char* c = input + tok->startIndex;
	//reading until you encounter '.' and digits that follow it
	//only one period is allowed, breaks if another is encountered
	while(isdigit(*c) || *c == '.'){
		if(periodRead && *c == '.'){
			break;
		} else if(*c == '.'){
			++periodRead;
		}
		++tok->endIndex;
		++c;
	}
	//if you run into an exponent
	char next = *(c+1);
	if((*c == 'e' || *c == 'E') && (next == '-' || next == '+' || isdigit(next))){
		++tok->endIndex;
		++c;
		//read + or - if they are there
		if(*c == '-' || *c == '+'){
			++tok->endIndex;
			++c;
		}
		//reading exponent digits
		while(isdigit(*c)){
			++tok->endIndex;
			++c;
		}
	}
}
//Determines if a character is allowed to be inside of a hexadecimal number after "0x"
int isHexChar(char c){
	if(isdigit(c) || c == 'a' || c == 'A' || c == 'b' || c == 'B' || c == 'c' || c == 'C' || c == 'd' || c == 'D' || c == 'e' || c == 'E' || c == 'f' || c == 'F'){
		return 1;
	}
	return 0;
}
//Finds out how many characters a hexadecimal token is
void processHexadecimalInt(token* tok, char* input){
	char* c = input + tok->startIndex + 2;
	tok->endIndex += 2;
	//assuming the first 2 chars are '0x'
	while(isHexChar(*c)){
		++tok->endIndex;
		++c;
	}
}
//Finds out how many characters a decimal token is
//Switches type to float if necessary
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
		processFloat(tok, input);
	}
}
//Finds out how many characters an octal token is
//Switches type to float if necessary
void processOctalInt(token* tok, char* input){
	char* c = input + tok->startIndex;
	while(isdigit(*c)){
		//switching type to decimal if character is not allowed in an octal but is
		//allowed to be a decimal
		if(*c == '8' || *c == '9'){
			tok->endIndex = tok->startIndex;
			tok->type = decimalInt;
			processDecimalInt(tok, input);
			return;
		}
		++tok->endIndex;
		++c;
	}
	//switching type to float if the terminating character is a '.'
	if(*c == '.'){
		tok->endIndex = tok->startIndex;
		tok->type = floatInt;
		processFloat(tok, input);
	}
}
//Finds out how many characters a word token is 
void processWord(token* tok, char* input){
	char* c = input + tok->startIndex;
	//edge case where the word is sizeof, change type
	if(c[0] == 's' && c[1] == 'i' && c[2] == 'z' && c[3] == 'e' && c[4] == 'o' && c[5] == 'f'){
		tok->endIndex += 6;
		tok->type = op;
		tok->opType = sizeOf;
		return;
	}
	//keep reading characters and incrementing the tokens ending index
	//until a character is not allowed to be in a word type
	while(isalnum(*c)){
		++tok->endIndex;
		++c;
	}
}
//Assumes what the initial type should be of the current token and sets it
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
//Gets a string representation of the operator type
char* getOperatorString(token* tok){
	switch(tok->opType){
		case left_parenthesis:
			return "left parenthesis";
			break;
		case right_parenthesis:
			return "right parenthesis";
			break;
		case left_bracket:
			return "left bracket";
			break;
		case right_bracket:
			return "right bracket";
			break;
		case structure_member:
			return "structure member";
			break;
		case structure_pointer:
			return "structure_pointer";
			break;
		case sizeOf:
			return "sizeof";
			break;
		case comma:
			return "comma";
			break;
		case negate:
			return "negate";
			break;
		case ones_complement:
			return "1s complement";
			break;
		case shift_right:
			return "shift right";
			break;
		case shift_left:
			return "shift left";
			break;
		case bitwise_XOR:
			return "bitwise XOR";
			break;
		case bitwise_OR:
			return "bitwise OR";
			break;
		case increment:
			return "increment";
			break;
		case decrement:
			return "decrement";
			break;
		case addition:
			return "addition";
			break;
		case division:
			return "division";
			break;
		case modulus:
			return "modulus";
			break;
		case logical_OR:
			return "logical OR";
			break;
		case logical_AND:
			return "logical AND";
			break;
		case conditional_true:
			return "conditional true";
			break;
		case conditional_false:
			return "conditional false";
			break;
		case equality_test:
			return "equality test";
			break;
		case inequality_test:
			return "inequality test";
			break;
		case less_than_test:
			return "less than test";
			break;
		case greater_than_test:
			return "greater than test";
			break;
		case less_than_or_equal_test:
			return "less than or equal test";
			break;
		case greater_than_or_equal_test:
			return "greater than or equal test";
			break;
		case assignment:
			return "assignment";
			break;
		case plus_equals:
			return "plus equals";
			break;
		case minus_equals:
			return "minus equals";
			break;
		case times_equals:
			return "times equals";
			break;
		case divide_equals:
			return "divide equals";
			break;
		case mod_equals:
			return "mod equals";
			break;
		case shift_right_equals:
			return "shift right equals";
			break;
		case shift_left_equals:
			return "shift left equals";
			break;
		case bitwise_AND_equals:
			return "bitwise AND equals";
			break;
		case bitwise_XOR_equals:
			return "bitwise XOR equals";
			break;
		case bitwise_OR_equals:
			return "bitwise OR equals";
			break;
		case AND_address_operator:
			return "AND/address operator";
			break;
		case minus_subtract_operator:
			return "minus/subtract operator";
			break;
		case multiply_dereference_operator:
			return "multiply/dereference operator";
			break;
		case unknown:
			return "unknown character";
			break;
		default:
			return "";
	}
}
//Gets a string representation of the token type
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
			return "hexadecimal integer";
			break;
		case floatInt:
			return "floating point";
			break;
		case op:
			return getOperatorString(tok);
			break;
		default: "";
	}
}
//Finds out what the type of token is and where the token starts/ends
void processToken(token* curToken, char* input, int startIndex){
	//initial endIndex of the token starts at the startIndex of the token
	curToken->startIndex = startIndex;
	curToken->endIndex = startIndex;
	//assumes the type that the token should start being read as
	processInitialChar(curToken, input);
	//based on the assumed type, how should we continue to read the token
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
			processOperator(curToken, input);
			break;
	}
	
}
//Prints the tokens of the input string
//if malloc fails then "Memory allocation error" is printed and the function returns
void printTokens(char* input){
	int i = 0;
	token* curToken = (token*)malloc(sizeof(token));
	if(curToken == NULL){
		printf("Memory allocation error\n");
		return;
	}
	while(input[i] != '\0'){
		//skips whitespace
		if(isspace(input[i])){
			++i;
			continue;
		}
		//Fill in info about the current token
		processToken(curToken, input, i);
		//Create the string that will be printed
		int tokenLength = curToken->endIndex - curToken->startIndex;
		char* str = (char*)malloc(sizeof(char)*tokenLength + 1);
		if(str == NULL){
			printf("Memory allocation error\n");
			return;
		}
		memcpy(str, &input[curToken->startIndex], tokenLength);
		str[tokenLength] = '\0';
		char* type_str = getTypeString(curToken);
		//Print out the current token
		printf("%s: \"%s\"\n", type_str, str);
		free(str);
		i = curToken->endIndex;
	}
	free(curToken);
}
//Main function that prints tokens for each command line argument provided
int main(int argc, char* argv[]){
	int i;
	for(i = 1; i < argc; ++i){
		printTokens(argv[i]);
	}
}
