/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xbarte14
 * Names: Bronislav Bárteček
 */
/**
 * @lexical_analysis
 * Lexical analysis documentation
 */
#ifndef IFJ_BBJK__LEXICAL_ANALYSIS_H
#define IFJ_BBJK__LEXICAL_ANALYSIS_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "buffer.h"
#include "exit.h"

/**
 * @typedef lexical analysis enums
 * @{
 */

/**
 * Enum for states in state machine
 */
typedef enum{
	STATE_START,
	STATE_LESS,
	STATE_MORE,
	STATE_SLASH,
	STATE_BLOCK_COMMENT,
	STATE_BLOCK_COMMENT_SLASH,
	STATE_LINE_COMMENT,
	STATE_ID_OR_RESERVED,
	STATE_INTEGER,
	STATE_DOUBLE_DOT,
	STATE_DOUBLE_EXP,
	STATE_DOUBLE_DOT_EXP,
	STATE_EXPLMARK,
	STATE_STRING_LITERAL
}TState;

/**
 * Enum for token->type
 */
typedef enum{
	//string literal
	/* 0 */ TYPE_STRING_LITERAL,

	//special charakters
	/* 1  */ TYPE_UNVALID,
	/* 2  */ TYPE_SEMICOLON,

	//operators
	/* 3  */ TYPE_PLUS,
	/* 4  */ TYPE_MINUS,
	/* 5  */ TYPE_MUL,
	/* 6  */ TYPE_SLASH,
	/* 7  */ TYPE_BACKSLASH,
	/* 8  */ TYPE_EQUAL,
	/* 9  */ TYPE_LESS,
	/* 10 */ TYPE_MORE,
	/* 11 */ TYPE_EXPLMARK,
	/* 12 */ TYPE_MORE_EQUAL,
	/* 13 */ TYPE_LESS_EQUAL,

	//EOL,EOF
	/* 14 */ TYPE_EOL,
	/* 15 */ TYPE_EOF,

	//bracket
	/* 16 */ TYPE_BRACKET_LEFT,
	/* 17 */ TYPE_BRACKET_RIGHT,

	//reserved words
	/* 18 */ TYPE_AS,
	/* 19 */ TYPE_ASC,
	/* 20 */ TYPE_DECLARE,
	/* 21 */ TYPE_DIM,
	/* 22 */ TYPE_DO,
	/* 23 */ TYPE_DOUBLE,
	/* 24 */ TYPE_ELSE,
	/* 25 */ TYPE_END,
	/* 26 */ TYPE_CHR,
	/* 27 */ TYPE_FUNCTION,
	/* 28 */ TYPE_IF,
	/* 29 */ TYPE_INPUT,
	/* 30 */ TYPE_INTEGER,
	/* 31 */ TYPE_LENGTH,
	/* 32 */ TYPE_LOOP,
	/* 33 */ TYPE_PRINT,
	/* 34 */ TYPE_RETURN,
	/* 35 */ TYPE_SCOPE,
	/* 36 */ TYPE_STRING,
	/* 37 */ TYPE_SUBSTR,
	/* 38 */ TYPE_THEN,
	/* 39 */ TYPE_WHILE,
	/* 40 */ TYPE_AND,
	/* 41 */ TYPE_BOOLEAN,
	/* 42 */ TYPE_CONTINUE,
	/* 43 */ TYPE_ELSEIF,
	/* 44 */ TYPE_EXIT,
	/* 45 */ TYPE_FALSE,
	/* 46 */ TYPE_FOR,
	/* 47 */ TYPE_NEXT,
	/* 48 */ TYPE_NOT,
	/* 49 */ TYPE_OR,
	/* 50 */ TYPE_SHARED,
	/* 51 */ TYPE_STATIC,
	/* 52 */ TYPE_TRUE,

	//ID
	/* 53 */ TYPE_IDENTIFIER,

	//numbers
	/* 54 */ TYPE_INTEGER_NUMBER,
	/* 55 */ TYPE_DOUBLE_NUMBER,

	/* 56 */ TYPE_COMMA,
	/* 57 */ TYPE_NOT_EQUAL,
}TToken_type;

/**
 * Reserved words
 * Just help char for function reserved_word
 */
extern char* reserved_words[];

/**
 * Reserved words
 * Just help char for function reserved_word
 */
extern TToken_type reserved_words_type[];
/** @} */

/**
 * @typedef lexical_analysis structures
 * \@{
 */
typedef struct _TToken{
	TToken_type type;
	void *data;
}TToken;
/** @} */

/**
 * @typedef lexical_analysis functions
 * \@{
 */

/**
 * Decide if is in buffer reserved word or identifier
 * @param token		pointer TToken, token where token->type will be changed in case of id
 * @param buffer	pointer TBuffer, buffer where string saved
 * @return integer value, 1 if reserved word, 0 if identifier
 * @pre buffer != NULL
 * @pre token != NULL
 */
int reserved_word(TToken *token,TBuffer * buffer);

/**
 * Allocate memory for structure TToken and initialize it with values
 * @return pointer to struct TToken
 */
TToken * token_init();

/**
 * Free token function
 * Free the memory allocated in token, it will free token->data too
 * @param token 	pointer to struct TToken which will be free
 * @pre token != NULL
 */
void token_free(TToken * token);

/**
 * Gives you token in structure TToken
 * If we have some token saved from previous token_unget function, it returns that
 * If we dont have stored token, it makes new one
 * token->type - type of token from TToken_type
 * token->data - pointer to data
 * 			SPECIAL DATA TYPE:
 * 				- type_string_literal 	: pointer to chars (with /0 at the end)
 * 				- type_identifier 		: pointer to chars (with /0 at the end)
 * 				- type_integer_number 	: pointer to integer
 * 				- type_double_number 	: pointer to double
 * 				- ELSE 					: NULL
 * @return pointer to struct TToken
 */
TToken * token_get();

/**
 * Gives you token in structure TToken and save token
 * If we have some token saved from previous token_unget function, it returns that and save new one
 * If we dont have saved token, it makes new one and save token
 * token->type - type of token from TToken_type
 * token->data - pointer to data
 * 			SPECIAL DATA TYPE:
 * 				- type_string_literal 	: pointer to chars (with /0 at the end)
 * 				- type_identifier 		: pointer to chars (with /0 at the end)
 * 				- type_integer_number 	: pointer to integer
 * 				- type_double_number 	: pointer to double
 * 				- ELSE 					: NULL
 * @param unget_token	!= NULL, we save token
 * @return pointer to struct TToken
 */
TToken * token_unget(TToken * unget_token);

/**
 * DO NOT CALL THIS FUNCTION, CALL token_get(); OR token_unget(TToken * unget_token) INSTEAD
 * Gives you token in structure TToken
 * token->type - type of token from TToken_type
 * token->data - pointer to data
 * 			SPECIAL DATA TYPE:
 * 				- type_string_literal 	: pointer to chars (with /0 at the end)
 * 				- type_identifier 		: pointer to chars (with /0 at the end)
 * 				- type_integer_number 	: pointer to integer
 * 				- type_double_number 	: pointer to double
 * 				- ELSE 					: NULL
 * @param unget_token	!= NULL, we save token
 * @return pointer to struct TToken
 */
TToken *token_get_func(TToken * unget_token);

/** @} */

#endif
