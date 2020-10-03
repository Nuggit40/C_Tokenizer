# C_Tokenizer
## Program Information
The program accepts user input strings of characters, numbers, punctuation, and some functions and returns tokens and token types. 
## Token types
- *Alphabetic Characters and Word Strings*
- *Decimal and Int Numbers*
- *Hex and Octal Numbers*
- *Operators*
## Code Example
```
void processWord(token* tok, char* input){
	char* c = input + tok->startIndex;
	//edge case where the word is sizeof, change type
	if(c[0] == 's' && c[1] == 'i' && c[2] == 'z' && c[3] == 'e' && c[4] == 'o' && c[5] == 'f'){
		tok->endIndex += 6;
		tok->type = op;
		tok->opType = sizeOf;
		return;
	}

```
## Resources
[C Library - <ctype.h>](https://www.tutorialspoint.com/c_standard_library/ctype_h.htm)
Assignment 0: CS214
