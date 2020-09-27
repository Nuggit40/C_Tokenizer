#ifndef _TOKEN_H
#define _TOKEN_H
typedef enum _tokenType{
	word, decimalInt, octalInt, hexadecimalInt, floatInt, op
} tokenType;
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
	multiply_dereference_operator
} operatorType;
typedef struct _token{
	int startIndex;
	int endIndex;
	tokenType type;
	operatorType opType;
} token;
#endif
