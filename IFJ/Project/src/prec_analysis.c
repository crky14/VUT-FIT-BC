/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#include "semantic.h"
#include "prec_analysis.h"

const int precedence_table[TABLE_ROWS][TABLE_COLS] = {
//	 +  -  *  /  \  i  (  )  C  $
	{R, R, S, S, S, S, S, R, R, R},	// "+","-"
	{R, R, R, R, R, S, S, R, R, R},	// "*","/"
	{R, R, S, S, R, S, S, R, R, R}, // "\"
	{R, R, R, R, R, E, E, R, R, R},	// "i"
	{S, S, S, S, S, S, S, H, S, E},	// "("
	{R, R, R, R, R, E, E, R, R, R},	// ")"
	{S, S, S, S, S, S, S, R, E, R},	// "comparatos"
	{S, S, S, S, S, S, S, E, S, F}	// "$"
};



prec_operations check_table(TStack *stack, TToken *token) {
	//operacia ktora sa bude vykonavat
	TStack_itmtype term = stack_first_term(stack);
	if(term == S_ERROR) {
		return E;
	}
	int col;
	switch(token->type){
		case TYPE_PLUS:
			col = 0;
			break;
		case TYPE_MINUS:
			col = 1;
			break;
		case TYPE_MUL:
			col = 2;
			break;
		case TYPE_SLASH:
			col = 3;
			break;
		case TYPE_BACKSLASH:
			col = 4;
			break;
		case TYPE_INTEGER_NUMBER:
		case TYPE_DOUBLE_NUMBER:
		case TYPE_STRING_LITERAL:
		case TYPE_IDENTIFIER:
			col = 5;
			break;
		case TYPE_BRACKET_LEFT:
			col = 6;
			break;
		case TYPE_BRACKET_RIGHT:
			col = 7;
			break;
		case TYPE_EQUAL:
		case TYPE_NOT_EQUAL:
		case TYPE_LESS:
		case TYPE_MORE:
		case TYPE_MORE_EQUAL:
		case TYPE_LESS_EQUAL:
			col = 8;
			break;
		case TYPE_EOL:
		case TYPE_SEMICOLON:
		case TYPE_THEN:			
			col = 9;
			break;
		case TYPE_EOF:
		default:
			return E;
	}

	switch(term) {
		case S_PLUS:
		case S_MINUS:
			return precedence_table[0][col];
		case S_MULTI:
		case S_DIV:
			return precedence_table[1][col];
		case S_INT_DIV:
			return precedence_table[2][col];
		case S_ID:	
			return precedence_table[3][col];
		case S_BRACELET_LEFT:	
			return precedence_table[4][col];
		case S_BRACELET_RIGHT:	
			return precedence_table[5][col];
		case S_LESS:
		case S_MORE:
		case S_EQUAL:
		case S_NOT_EQUAL:
		case S_MORE_EQUAL:
		case S_LESS_EQUAL:
			return precedence_table[6][col];
		case S_EOL:
			return precedence_table[7][col];
		default:
			return E;
	}
}

TStack_itmtype map_token(TToken *token) {
	switch(token->type){
		case TYPE_PLUS:
			return S_PLUS;
		case TYPE_MINUS:
			return S_MINUS;
		case TYPE_MUL:
			return S_MULTI;
		case TYPE_SLASH:
			return S_DIV;
		case TYPE_BACKSLASH:
			return S_INT_DIV;
		case TYPE_INTEGER_NUMBER:
		case TYPE_DOUBLE_NUMBER:
		case TYPE_STRING_LITERAL:
		case TYPE_IDENTIFIER:
			return S_ID;
		case TYPE_BRACKET_LEFT:
			return S_BRACELET_LEFT;
		case TYPE_BRACKET_RIGHT:
			return S_BRACELET_RIGHT;
		case TYPE_NOT_EQUAL:
			return S_NOT_EQUAL;
		case TYPE_EQUAL:;
			return S_EQUAL;
		case TYPE_LESS:
			return S_LESS;
		case TYPE_MORE:
			return S_MORE;
		case TYPE_MORE_EQUAL:
			return S_MORE_EQUAL;
		case TYPE_LESS_EQUAL:
			return S_LESS_EQUAL;
		case TYPE_EOL:
		case TYPE_SEMICOLON:
		case TYPE_THEN:
			return S_EOL;
		default:
			return -1;
	}
}

void execute_rule(TStack *stack, int rule_len) {
	for(int i = 0; i < rule_len + 1; i++) {
		if(stack->First == NULL) {
			stack_dispose(stack);
			free(stack);
			exit_program(2);
		}
		stack_pop(stack);
	}
}

void check_rules(TStack *stack, TStack_itmptr *rule, int rule_len) {
	if(rule_len == 1) {
		if(rule[0]->type == S_ID) {
			sem_types expr_type = rule[0]->sem_type;
			execute_rule(stack, rule_len);
			stack_push(stack, S_EXPR, expr_type);
			return;
		}
	}
	else if(rule_len == 3){
		if(rule[0]->type == S_EXPR && rule[2]->type == S_EXPR) {
			if(rule[1]->type != S_ID && rule[1]->type != S_EXPR && rule[1]->type != S_TOP) {
				sem_types expr_type = semantic_expression(rule);
				if(expr_type == NOT_DEFINED) {
					stack_dispose(stack);
					free(stack);
					exit_program(4);
				}
				execute_rule(stack, rule_len);
				stack_push(stack, S_EXPR, expr_type);
				return;
			}
		}
		else if(rule[0]->type == S_BRACELET_RIGHT && rule[1]->type == S_EXPR && rule[2]->type == S_BRACELET_LEFT){
			sem_types expr_type = rule[1]->sem_type;
			execute_rule(stack, rule_len);
			stack_push(stack, S_EXPR, expr_type);
			return;
		}
	}
	stack_dispose(stack);
	free(stack);
	exit_program(2);
}

int parse_expression(TToken *token) {
	if(token->type == TYPE_EOL || token->type == TYPE_EOF || token->type == TYPE_SEMICOLON || token->type == TYPE_THEN) {
		exit_program(2);
	}
	TToken *expr_token = token;
	//vytvorenie precedencneho stacku
	TStack *stack = malloc(sizeof(TStack)); // CHCECK
	if(stack == NULL) {
		exit_program(99);
	}

	stack_init(stack);
	stack_push(stack, S_EOL, NOT_DEFINED);

	gen_createframe();

	prec_operations operation;
	do{
		operation = check_table(stack, expr_token);
		if(operation == S) {
			stack_newtop(stack);
			if(map_token(expr_token) == S_ID) {
				if(expr_token->type == TYPE_IDENTIFIER) {
					sem_types id_type = semantic_identifier(expr_token->data);
					if(id_type == NOT_DEFINED) {
						stack_dispose(stack);
						free(stack);
						exit_program(3);
					}
					stack_push(stack, map_token(expr_token), id_type);
				}
				else if(expr_token->type == TYPE_INTEGER_NUMBER) 
					stack_push(stack, map_token(expr_token), INTEGER);
				else if(expr_token->type == TYPE_DOUBLE_NUMBER)
					stack_push(stack, map_token(expr_token), DOUBLE);
				else if(expr_token->type == TYPE_STRING_LITERAL)
					stack_push(stack, map_token(expr_token), STRING);
				gen_datapush(expr_token);	
			}
			else {
				stack_push(stack, map_token(expr_token), NOT_DEFINED);
			}
		}
		else if(operation == R) {
			int count = 0;
			TStack_itmptr rule[MAX_RULE_LENGTH] = {NULL};
			TStack_itmptr tmp = stack->First;
			while(tmp->type != S_TOP) {
				rule[count] = tmp;
				count++;
				tmp = tmp->next;
			}

			check_rules(stack, rule, count);

			if(check_table(stack, expr_token) == R || check_table(stack, expr_token) == E) {
				continue;
			}
			if(expr_token->type != TYPE_EOL && expr_token->type != TYPE_THEN && expr_token->type != TYPE_SEMICOLON) {
				if(expr_token->type != TYPE_BRACKET_RIGHT) {
					stack_newtop(stack);
				}
				stack_push(stack, map_token(expr_token), NOT_DEFINED);
			}
		}
		else if(operation == H) {
			stack_push(stack, map_token(expr_token), NOT_DEFINED);
		}
		else if(operation == E){
			stack_dispose(stack);
			free(stack);
			exit_program(2);
		}
		
		if((expr_token->type != TYPE_EOL) && (expr_token->type != TYPE_THEN) && (expr_token->type != TYPE_SEMICOLON)) {
			expr_token = token_get();
			if((expr_token->type != TYPE_EOL) && (expr_token->type != TYPE_THEN) && (expr_token->type != TYPE_SEMICOLON))
				token_save(expr_token);
		}
	}while(check_table(stack, expr_token) != F);

	TStack_itmtype result_type;
	result_type = stack->First->sem_type;
	token_unget(expr_token);
	stack_dispose(stack);
	free(stack);
	return result_type;
}







