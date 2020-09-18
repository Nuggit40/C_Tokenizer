#ifndef _TOKEN_H
#define _TOKEN_H
typedef enum _tokenType{
	word, decimalInt, octalInt, hexadecimalInt, floatInt, op
} tokenType;
typedef struct _token{
	int startIndex;
	int endIndex;
	tokenType type;
} token;
#endif
