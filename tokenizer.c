#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef enum _tokenType{
	word, decimalInt, octalInt, hexadecimalInt, floatInt, op, kw
} tokenType;
typedef enum _keyword{
	_char,
	_auto,	
	_double,
	_int,
	_struct,
	_break,
	_else,
	_long,
	_switch,
	_case,
	_enum,
	_register,
	_typedef,
	_extern,
	_return,
	_union,
	_continue,
	_for,
	_signed,
	_void,
	_do,
	_if,
	_static,
	_while,
	_default,
	_goto,
	_volatile,
	_const,
	_float,
	_short,
	_unsigned,

} keywordType;
typedef enum _operatorType{
	left_parenthesis,
	right_parenthesis,
	left_bracket,
	right_bracket,
	structure_member,
	structure_pointer,
	sizeOf,
	comma,
	negate,
	ones_complement,
	shift_right,
	shift_left,
	bitwise_XOR,
	bitwise_OR,
	increment,
	decrement,
	addition,
	division,
	modulus,
	logical_OR,
	logical_AND,
	conditional_true,
	conditional_false,
	equality_test,
	inequality_test,
	less_than_test,
	greater_than_test,
	less_than_or_equal_test,
	greater_than_or_equal_test,
	assignment,
	plus_equals,
	minus_equals,
	times_equals,
	divide_equals,
	mod_equals,
	shift_right_equals,
	shift_left_equals,
	bitwise_AND_equals,
	bitwise_XOR_equals,
	bitwise_OR_equals,
	AND_address_operator,
	minus_subtract_operator,
	multiply_dereference_operator,
	unknown
} operatorType;
typedef struct _token{
	int startIndex;
	int endIndex;
	tokenType type;
	operatorType opType;
	keywordType kwType;
} token;
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
//checks if the word being processes is a keyword and processes the keyword if applicable
int processKeyword(token* tok, char* input){
	char* c = input + tok->startIndex;
	if(c[0] == 'a' && c[1] == 'u' && c[2] == 't' && c[3] == 'o'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _auto;
		return 1;
	}
	else if(c[0] == 'b' && c[1] == 'r' && c[2] == 'e' && c[3] == 'a' && c[4] == 'k'){
		tok->endIndex += 5;
		tok->type = kw;
		tok->kwType = _break;
		return 1;
	}
	else if(c[0] == 'c' && c[1] == 'a' && c[2] == 's' && c[3] == 'e'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _case;
		return 1;
	}
	else if(c[0] == 'd' && c[1] == 'o'){
		tok->endIndex += 2;
		tok->type = kw;
		tok->kwType = _do;
		return 1;
	}
	else if(c[0] == 'c' && c[1] == 'o' && c[2] == 'n' && c[3] == 't' && c[4] == 'i' && c[5] == 'n' && c[6] == 'u' && c[7] == 'e'){
		tok->endIndex += 8;
		tok->type = kw;
		tok->kwType = _continue;
		return 1;
	}
	else if(c[0] == 'd' && c[1] == 'e' && c[2] == 'f' && c[3] == 'a' && c[4] == 'u' && c[5] == 'l' && c[6] == 't'){
		tok->endIndex += 7;
		tok->type = kw;
		tok->kwType = _default;
		return 1;
	}
	else if(c[0] == 'c' && c[1] == 'o' && c[2] == 'n' && c[3] == 's' && c[4] == 't'){
		tok->endIndex += 5;
		tok->type = kw;
		tok->kwType = _const;
		return 1;
	}
	else if(c[0] == 'd' && c[1] == 'o' && c[2] == 'u' && c[3] == 'b' && c[4] == 'l' && c[5] == 'e'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _double;
		return 1;
	}
	else if(c[0] == 'e' && c[1] == 'l' && c[2] == 's' && c[3] == 'e'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _else;
		return 1;
	}
	else if(c[0] == 'e' && c[1] == 'n' && c[2] == 'u' && c[3] == 'm'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _enum;
		return 1;
	}
	else if(c[0] == 'e' && c[1] == 'x' && c[2] == 't' && c[3] == 'e' && c[4] == 'r' && c[5] == 'n'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _extern;
		return 1;
	}
	else if(c[0] == 'f' && c[1] == 'o' && c[2] == 'r'){
		tok->endIndex += 3;
		tok->type = kw;
		tok->kwType = _for;
		return 1;
	}
	else if(c[0] == 'i' && c[1] == 'f'){
		tok->endIndex += 2;
		tok->type = kw;
		tok->kwType = _if;
		return 1;
	}
	else if(c[0] == 'g' && c[1] == 'o' && c[2] == 't' && c[3] == 'o'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _goto;
		return 1;
	}
	else if(c[0] == 'f' && c[1] == 'l' && c[2] == 'o' && c[3] == 'a' && c[4] == 't'){
		tok->endIndex += 5;
		tok->type = kw;
		tok->kwType = _float;
		return 1;
	}
	else if(c[0] == 'i' && c[1] == 'n' && c[2] == 't'){
		tok->endIndex += 3;
		tok->type = kw;
		tok->kwType = _int;
		return 1;
	}
	else if(c[0] == 'c' && c[1] == 'h' && c[2] == 'a' && c[3] == 'r'){
		tok->endIndex +=4;
		tok->type = kw;
		tok->kwType = _char;
		return 1;
	}
	else if(c[0] == 's' && c[1] == 'i' && c[2] == 'g' && c[3] == 'n' && c[4] == 'e' && c[5] == 'd'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _signed;
		return 1;	
	}
	else if(c[0] == 's' && c[1] == 'h' && c[2] == 'o' && c[3] == 'r'&& c[4] == 't' ){
		tok->endIndex += 5;
		tok->type = kw;
		tok->kwType = _short;
		return 1;
	}
	else if(c[0] == 'l' && c[1] == 'o' && c[2] == 'n' && c[3] == 'g'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _long;
		return 1;
	}
	else if(c[0] == 'u' && c[1] == 'n' && c[2] == 's' && c[3] == 'i' && c[4] == 'g' && c[5] == 'n' && c[6] == 'e' && c[7] == 'd'){
		tok->endIndex += 8;
		tok->type = kw;
		tok->kwType = _unsigned;
		return 1;
	}
	else if(c[0] == 'v' && c[1] == 'o' && c[2] == 'l' && c[3] == 'a' && c[4] == 't' && c[5] == 'i' && c[6] == 'l' && c[7] == 'e'){
		tok->endIndex += 8;
		tok->type = kw;
		tok->kwType = _volatile;
		return 1;
	}
	else if(c[0] == 'w' && c[1] == 'h' && c[2] == 'i' && c[3] == 'l' && c[4] == 'e'){
		tok->endIndex += 5;
		tok->type = kw;
		tok->kwType = _while;
		return 1;
	}
	else if(c[0] == 'v' && c[1] == 'o' && c[2] == 'i' && c[3] == 'd'){
		tok->endIndex += 4;
		tok->type = kw;
		tok->kwType = _void;
		return 1;
	}
	else if(c[0] == 'u' && c[1] == 'n' && c[2] == 'i' && c[3] == 'o' && c[4] == 'n'){
		tok->endIndex += 5;
		tok->type = kw;
		tok->kwType = _union;
		return 1;
	}
	else if(c[0] == 't' && c[1] == 'y' && c[2] == 'p' && c[3] == 'e' && c[4] == 'd' && c[5] == 'e' && c[6] == 'f'){
		tok->endIndex += 7;
		tok->type = kw;
		tok->kwType = _typedef;
		return 1;
	}
	else if(c[0] == 's' && c[1] == 'w' && c[2] == 'i' && c[3] == 't' && c[4] == 'c' && c[5] == 'h'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _switch;
		return 1;
	}
	else if(c[0] == 's' && c[1] == 't' && c[2] == 'r' && c[3] == 'u' && c[4] == 'c' && c[5] == 't'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _struct;
		return 1;
	}
	else if(c[0] == 's' && c[1] == 't' && c[2] == 'a' && c[3] == 't' && c[4] == 'i' && c[5] == 'c'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _static;
		return 1;
	}
	else if(c[0] == 'r' && c[1] == 'e' && c[2] == 't' && c[3] == 'u' && c[4] == 'r' && c[5] == 'n'){
		tok->endIndex += 6;
		tok->type = kw;
		tok->kwType = _return;
		return 1;
	}
	else if(c[0] == 'r' && c[1] == 'e' && c[2] == 'g' && c[3] == 'i' && c[4] == 's' && c[5] == 't' && c[6] == 'e' && c[7] == 'r'){
		tok->endIndex += 8;
		tok->type = kw;
		tok->kwType = _register;
		return 1;
	}
	return 0;
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
	//checking if the word happens to be a keyword and process that keyword
	if(processKeyword(tok, input)){
		return;
	}
	//keep reading characters and incrementing the tokens ending index
	//until a character is not allowed to be in a word type
	while(isalnum(*c)){
		++tok->endIndex;
		++c;
	}
}
char* getKeywordString(token* tok){
	switch(tok->kwType){
		case _auto:
			return "auto";
			break;
		case _break:
			return "break";
			break;
		case _case:
			return "case";
			break;
		case _char:
			return "char";
			break;
		case _continue:
			return "continue";
			break;
		case _do:
			return "do";
			break;
		case _default:
			return "default";
			break;
		case _const:
			return "const";
			break;
		case _double:
			return "double";
			break;
		case _else:
			return "else";
			break;
		case _enum:
			return "enum";
			break;
		case _extern:
			return "extern";
			break;
		case _for:
			return "for";
			break;
		case _if:
			return "if";
			break;
		case _goto:
			return "goto";
			break;
		case _float:
			return "float";
			break;
		case _int:
			return "int";
			break;
		case _long:
			return "long";
			break;
		case _register:
			return "register";
			break;
		case _signed:
			return "signed";
			break;
		case _static:
			return "static";
			break;
		case _short:
			return "short";
			break;
		case _struct:
			return "struct";
			break;
		case _switch:
			return "switch";
			break;
		case _typedef:
			return "typedef";
			break;
		case _union:
			return "union";
			break;
		case _void:
			return "void";
			break;
		case _while:
			return "while";
			break;
		case _volatile:
			return "volatile";
			break;
		case _unsigned:
			return "unsigned";
			break;
		default:
			return "";
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
			return "structure pointer";
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
		case kw:
			return getKeywordString(tok);
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
		exit(EXIT_FAILURE);
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
			exit(EXIT_FAILURE);
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
	//printTokens("char");
	for(i = 1; i < argc; ++i){
		printTokens(argv[i]);
	}
}
