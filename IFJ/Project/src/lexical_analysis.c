/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xbarte14
 * Names: Bronislav Bárteček
 */


#include "lexical_analysis.h"


char* reserved_words[]={
		"as",
		"asc",
		"declare",
		"dim",
		"do",
		"double",
		"else",
		"end",
		"chr",
		"function",
		"if",
		"input",
		"integer",
		"length",
		"loop",
		"print",
		"return",
		"scope",
		"string",
		"substr",
		"then",
		"while",
		"and",
		"boolean",
		"continue",
		"elseif",
		"exit",
		"false",
		"for",
		"next",
		"not",
		"or",
		"shared",
		"static",
		"true",
};

TToken_type reserved_words_type[]={
		TYPE_AS,
		TYPE_ASC,
		TYPE_DECLARE,
		TYPE_DIM,
		TYPE_DO,
		TYPE_DOUBLE,
		TYPE_ELSE,
		TYPE_END,
		TYPE_CHR,
		TYPE_FUNCTION,
		TYPE_IF,
		TYPE_INPUT,
		TYPE_INTEGER,
		TYPE_LENGTH,
		TYPE_LOOP,
		TYPE_PRINT,
		TYPE_RETURN,
		TYPE_SCOPE,
		TYPE_STRING,
		TYPE_SUBSTR,
		TYPE_THEN,
		TYPE_WHILE,
		TYPE_AND,
		TYPE_BOOLEAN,
		TYPE_CONTINUE,
		TYPE_ELSEIF,
		TYPE_EXIT,
		TYPE_FALSE,
		TYPE_FOR,
		TYPE_NEXT,
		TYPE_NOT,
		TYPE_OR,
		TYPE_SHARED,
		TYPE_STATIC,
		TYPE_TRUE,
};

int reserved_word(TToken *token,TBuffer * buffer){
	for (int i = 0; i < 35; i++) {
		if (strcmp(reserved_words[i],buffer->data) == 0){
			token->type = reserved_words_type[i];
			return 1;
		}
	}
	return 0;
}

TToken * token_init(){
	TToken * token;
	token = malloc(sizeof(TToken));
	if (token == NULL){
		exit_program(99);
	}
	token->type=TYPE_UNVALID;
	token->data=NULL;
	return token;
}

void token_free(TToken * token){
	if (token == NULL){
		return;
	}
	else{
		if (token->data != NULL){
			free(token->data);
		}
		free(token);
	}
}

TToken * token_get(){
	return token_get_func(NULL);
}

TToken * token_unget(TToken * unget_token){
	return token_get_func(unget_token);
}

TToken *token_get_func(TToken * unget_token) {
	TToken *token;
	TState state = STATE_START;
	static TToken * previous_token = NULL;
	static int is_saved = 0;
	int c;
	TBuffer * buffer = NULL;
	int checker = 0;
	int checker2 = 0;
	int checker3 = 0;

	if (unget_token == NULL){	// we want new token
		if (is_saved){	// have we previous token saved?
			is_saved = 0;
			return previous_token;
		}
	}
	else{	//we want to store token and get new one
		is_saved = 1;
		previous_token = unget_token;
		return NULL;
	}

	token = token_init();

	while (1){
		c = getc(stdin);
		if (state != STATE_STRING_LITERAL){
			c = tolower(c);
		}

		switch (state){
			case STATE_START:
				while (isspace(c) && c != '\n' && c != EOF){
					c = tolower(getc(stdin));
				}
				switch (c) {
					case '+':
						token->type = TYPE_PLUS;
						return token;
					case '-':
						token->type = TYPE_MINUS;
						return token;
					case '*':
						token->type = TYPE_MUL;
						return token;
					case '/':
						state = STATE_SLASH;
						break;
					case '\\':
						token->type = TYPE_BACKSLASH;
						return token;
					case '=':
						token->type = TYPE_EQUAL;
						return token;
					case '<':
						state = STATE_LESS;
						break;
					case '>':
						state = STATE_MORE;
						break;
					case '!':
						state = STATE_EXPLMARK;
						break;
					case '\'':
						state = STATE_LINE_COMMENT;
						break;
					case '\n':
						token->type = TYPE_EOL;
						return token;
					case EOF:
						token->type = TYPE_EOF;
						return token;
					case '(':
						token->type = TYPE_BRACKET_LEFT;
						return token;
						break;
					case ')':
						token->type = TYPE_BRACKET_RIGHT;
						return token;
						break;
					case ';':
						token->type = TYPE_SEMICOLON;
						return token;
						break;
					case ',':
						token->type = TYPE_COMMA;
						return token;
						break;
					default:
						if(isdigit(c)){
							buffer = buffer_init();
							buffer_add(c,buffer);
							state = STATE_INTEGER;
						}
						else if(isalpha(c)||(c == '_')){
							buffer = buffer_init();
							buffer_add(c,buffer);
							state = STATE_ID_OR_RESERVED;
						}
						else{
							token_free(token);
							exit_program(1);
						}
				}
				break;
			case STATE_MORE:
				switch (c) {
					case '=':
						token->type = TYPE_MORE_EQUAL;
						return token;
					default:
						ungetc(c, stdin);
						token->type = TYPE_MORE;
						return token;
				}
				break;
			case STATE_LESS:
				switch (c){
					case '=':
						token->type = TYPE_LESS_EQUAL;
						return token;
					case '>':
						token->type = TYPE_NOT_EQUAL;
						return token;
					default:
						ungetc(c, stdin);
						token->type = TYPE_LESS;
						return token;
				}
				break;
			case STATE_SLASH:
				switch (c) {
					case '\'':
						state = STATE_BLOCK_COMMENT;
						break;
					default:
						token->type = TYPE_SLASH;
						ungetc(c, stdin);
						return token;
				}
				break;
			case STATE_BLOCK_COMMENT:
				while (c != '\'') {
					c = tolower(getc(stdin));
					if (c == EOF ){
						token_free(token);
						exit_program(1);
					}
				}
				state = STATE_BLOCK_COMMENT_SLASH;
				break;
			case STATE_BLOCK_COMMENT_SLASH:
				switch (c) {
					case '/':
						state = STATE_START;
						break;
					default:
						state = STATE_BLOCK_COMMENT;
				}
				break;
			case STATE_LINE_COMMENT:
				while (c != '\n' && c != EOF){
					c = getc(stdin);
				}
				token->type = TYPE_EOL;
				return token;
				break;
			case STATE_ID_OR_RESERVED:
				while (isalpha(c)||isdigit(c)||(c == '_')){
					buffer_add(c,buffer);
					c = tolower(getc(stdin));
				}
				ungetc(c,stdin);
				if (reserved_word(token,buffer)){
					buffer_free(buffer);
					return token;
				}
				else{
					token->type = TYPE_IDENTIFIER;
					token->data = buffer_to_char(buffer);
					buffer_free(buffer);
					return token;
				}
				break;
			case STATE_INTEGER:
				if (c == 'e'){
					buffer_add(c,buffer);
					checker = 0;
					checker2 = 0;
					state = STATE_DOUBLE_EXP;
				}
				else if (c == '.'){
					buffer_add(c,buffer);
					checker = 0;
					state = STATE_DOUBLE_DOT;
				}
				else if (isdigit(c)){
					buffer_add(c,buffer);
				}
				else{
					ungetc(c,stdin);
					token->type = TYPE_INTEGER_NUMBER;
					token->data = buffer_to_int(buffer);
					buffer_free(buffer);
					return token;
				}
				break;
			case STATE_DOUBLE_DOT:
				if (c == 'e'){
					if (checker == 0){
						token_free(token);
						exit_program(1);
					}
					buffer_add(c,buffer);
					state = STATE_DOUBLE_DOT_EXP;
					checker = 0;
					checker2 = 0;
				}
				else if (isdigit(c)){
					buffer_add(c,buffer);
					checker++;
				}
				else{
					ungetc(c,stdin);
					if (checker == 0){
						token_free(token);
						exit_program(1);
					}
					token->type = TYPE_DOUBLE_NUMBER;
					token->data = buffer_to_double(buffer);
					buffer_free(buffer);
					return token;
				}
				break;
			case STATE_DOUBLE_DOT_EXP:
				if (isdigit(c)){
					buffer_add(c,buffer);
					checker++;
				}
				else if (c == '+'||c =='-'){
					buffer_add(c,buffer);
					if (checker2 != 0){
						token_free(token);
						exit_program(1);
					}
					checker2++;
				}
				else{
					ungetc(c,stdin);
					if (checker == 0){
						token_free(token);
						exit_program(1);
					}
					token->type = TYPE_DOUBLE_NUMBER;
					token->data = buffer_to_double(buffer);
					buffer_free(buffer);
					return token;
				}
				break;
			case STATE_DOUBLE_EXP:
				if (isdigit(c)){
					buffer_add(c,buffer);
					checker++;
				}
				else if (c == '+'||c =='-'){
					buffer_add(c,buffer);
					if (checker2 != 0){
						token_free(token);
						exit_program(1);
					}
					checker2++;
				}
				else{
					ungetc(c,stdin);
					if (checker == 0){
						token_free(token);
						exit_program(1);
					}
					token->type = TYPE_DOUBLE_NUMBER;
					token->data = buffer_to_double(buffer);
					buffer_free(buffer);
					return token;
				}
				break;
			case STATE_EXPLMARK:
				if (c == '\"'){
					buffer = buffer_init();
					state = STATE_STRING_LITERAL;
				}
				else{
					token_free(token);
					exit_program(1);
				}
				break;
			case STATE_STRING_LITERAL:
				while (!((c == '\"') && (checker3 != '\\'))){
					if (c == EOF || c == '\n'|| c <= 31){
						token_free(token);
						exit_program(1);
					}
					buffer_add(c,buffer);
					checker3 = c;
					c = getc(stdin);
				}
				token->type = TYPE_STRING_LITERAL;
				token->data = buffer_to_char(buffer);
				buffer_free(buffer);
				return token;
				break;
		}
	}
}
