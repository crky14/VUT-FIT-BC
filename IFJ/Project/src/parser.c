/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xblask04
 * Names: Barbora Blašková
 */
#include <stdio.h>
#include <stdlib.h>

#include "lexical_analysis.h"

#include "debug.h"

#include "symtable.h"
#include "exit.h"

#include "semantic.h"
#include "prec_analysis.h"
#include "generator.h"
#include "parser.h"

#define DEBUG 0

int check_epsilon(TToken_type exp_type){
	int ret_val;
	TToken *token = token_get();
	debug_print("******In epsilon******\n");
	debug_print("Checking %d (next token) and %d(expected token)\n\n", token->type, exp_type);
	if(token->type == exp_type) ret_val = IS_EPSILON;
	if(token->type != exp_type) ret_val = NOT_EPSILON;
	token_unget(token);
	return ret_val;
}

int syntax_check(){

	if(program() == SYNTAX_ERR){
		return SYNTAX_ERR;
	} else {
		return SYNTAX_OK;
	}

}

int program(){
	debug_print("***Entering program****\n");

	if(check_epsilon(TYPE_EOF) == IS_EPSILON) return SYNTAX_ERR; //epsilon

	if(end_n() == SYNTAX_ERR) return SYNTAX_ERR;
	if(check_epsilon(TYPE_EOF) == IS_EPSILON) return SYNTAX_ERR; //epsilon
	if(functions() == SYNTAX_ERR) return SYNTAX_ERR;
	if(scope() == SYNTAX_ERR) return SYNTAX_ERR;
	if(end_n() == SYNTAX_ERR) return SYNTAX_ERR;

	TToken *token_eof = token_get();
	token_save(token_eof);
	if(token_eof->type != TYPE_EOF) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int scope(){
	debug_print("****Entering scope*****\n");
	if(check_epsilon(TYPE_EOF) == IS_EPSILON) return SYNTAX_OK; //epsilon 
	if(semantic_function_all() == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
	TToken *token_scope = token_get();
	token_save(token_scope);
	debug_print("Getting token of type %d\n", token_scope->type);
	if(token_scope->type != TYPE_SCOPE) return SYNTAX_ERR;
	gen_scope();
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(scope_content() == SYNTAX_ERR) return SYNTAX_ERR;

	TToken *token_end = token_get();
	token_save(token_end);
	if(token_end->type != TYPE_END) return SYNTAX_ERR;

	TToken *token_end_scope = token_get();
	token_save(token_end_scope);
	if(token_end_scope->type != TYPE_SCOPE) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int scope_content(){
	debug_print("***Entering scope_content***\n");
	if(body() == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int assignment(void *var_name){
	if(check_epsilon(TYPE_EOL) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	TToken *token_eq = token_get();
	token_save(token_eq);
	if(token_eq->type != TYPE_EQUAL) return SYNTAX_ERR;

	TToken *token_bracket = NULL;
	TToken *token_right_bracket = NULL;
	global_item *function = NULL;
	TToken *token = token_get();
	token_save(token);

	if(token->type == TYPE_IDENTIFIER || token->type == TYPE_LENGTH || token->type == TYPE_SUBSTR || token->type == TYPE_ASC || token->type == TYPE_CHR){
		token_bracket = token_get();
		if(token_bracket->type != TYPE_BRACKET_LEFT){ //we are not calling a function but an expression
			token_unget(token_bracket);
			int expr_type = parse_expression(token);
			if(semantic_assigment(var_name ,expr_type) == SEM_ERROR_COMP) exit_program(4);
			//gen_assignment(var_name);
		}else{ //we are calling a function
			char *func_name;
			if(token->type == TYPE_LENGTH) {
				function = create_function("length");
				func_name = "length";
			}
			else if(token->type == TYPE_SUBSTR){
				function = create_function("substr");
				func_name = "substr";
			}
			else if(token->type == TYPE_ASC){
				function = create_function("asc");
				func_name = "asc";
			}
			else if(token->type == TYPE_CHR){
				function = create_function("chr");
				func_name = "chr";
			}
			else{
				function = create_function(token->data);
				func_name = token->data;
			}
			if(function == NULL){
				exit_program(99);
			}
			token_save(token_bracket);
			if(params(function) == SYNTAX_ERR) return SYNTAX_ERR;
			token_right_bracket = token_get();
			token_save(token_right_bracket);
			if(token_right_bracket->type != TYPE_BRACKET_RIGHT) return SYNTAX_ERR;
			int ret = semantic_function_call(function);
			if(ret == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
			else if(ret == SEM_ERROR_UND) exit_program(SEM_ERROR_UND);
			else if(ret == SEM_ERROR_COMP) exit_program(SEM_ERROR_COMP);
			if(semantic_function_assigment(var_name, func_name) == SEM_ERROR_COMP) exit_program(4);
		}
	} else { //the first token was number/something else
		debug_print("Sending precedence token %d\n", token->type);
		int expr_type = parse_expression(token);
		if(semantic_assigment(var_name ,expr_type) == SEM_ERROR_COMP) exit_program(4);
		//gen_assignment(var_name);
	}
	return SYNTAX_OK;
}

int end(){
	debug_print("****Entering end****\n");
	TToken *token_eol = token_get();
	token_save(token_eol);
	debug_print("Getting token of type %d\n", token_eol->type);
	if(token_eol->type != TYPE_EOL) return SYNTAX_ERR;
	if(end_n() == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int end_n(){
	//EPSILON
	debug_print("***Entering end_n***\n");
	if(check_epsilon(TYPE_INPUT) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_PRINT) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_IF) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_DO) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_IDENTIFIER) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_DIM) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_DECLARE) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_FUNCTION) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_END) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_SCOPE) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_EOF) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_RETURN) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_ELSE) == IS_EPSILON) return SYNTAX_OK; //epsilon
	if(check_epsilon(TYPE_LOOP) == IS_EPSILON) return SYNTAX_OK; //epsilon


	TToken *token_eol = token_get();
	token_save(token_eol);

	debug_print("Getting token of type %d\n", token_eol->type);
	if(token_eol->type != TYPE_EOL) return SYNTAX_ERR;
	if(end_n() == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int body(){
	debug_print("***Entering body***\n");
	if(check_epsilon(TYPE_END) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(statement() == SYNTAX_ERR) return SYNTAX_ERR;
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(body() == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int if_body(int from_function, void *func_name, int* was_return){
	debug_print("***Entering if_body***\n");
	if(check_epsilon(TYPE_ELSE) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(statement_list(from_function, func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(if_body(from_function, func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int if_body_end(int from_function, void *func_name, int* was_return){
	debug_print("***Entering if_body_end***\n");
	if(check_epsilon(TYPE_END) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(statement_list(from_function, func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(if_body_end(from_function, func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int loop_body(){
	debug_print("***Entering loop_body***\n");
	if(check_epsilon(TYPE_LOOP) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(statement() == SYNTAX_ERR) return SYNTAX_ERR;
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(loop_body() == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int statement(){
	return statement_list(0, NULL, NULL);
}

int statement_list(int from_function, void *func_name, int* was_return){
	debug_print("***Entering statement***\n");
	debug_print("***from func is %d***\n", from_function);
	TToken *token = token_get();
	token_save(token);
	debug_print("Getting token of type %d\n", token->type);
	TToken *token_id = NULL;
	TToken *token_semicolon = NULL;
	TToken *token_then = NULL;
	TToken *token_else = NULL;
	TToken *token_end = NULL;
	TToken *token_if = NULL;
	TToken *token_while = NULL;
	TToken *token_loop = NULL;
	TToken *token_eq = NULL;
	TToken *token_as = NULL;
	TToken *token_expr = NULL;
	TToken *token_ret = NULL;
	int expr_type;

	switch(token->type){
		int expr_result;
		case TYPE_DIM:
			token_id = token_get();
			token_save(token_id);
			debug_print("In case dim: Getting token of type %d\n", token_id->type);
			if(token_id->type != TYPE_IDENTIFIER) return SYNTAX_ERR;
			variable_item *var = create_variable(token_id->data);
			if(var == NULL){
				exit_program(99);
			}
			if(semantic_variable(var) == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);

			token_as = token_get();
			debug_print("In case dim: Getting token of type %d\n", token_as->type);
			token_save(token_as);
			if(token_as->type != TYPE_AS) return SYNTAX_ERR;

			if(var_type(var) == SYNTAX_ERR) return SYNTAX_ERR;
			gen_var(var);
			add_variable(var);
			if(assignment(token_id->data) == SYNTAX_ERR) return SYNTAX_ERR;
			return SYNTAX_OK;
			break;
		case TYPE_INPUT:
			token_id = token_get();
			token_save(token_id);
			debug_print("In case input: Getting token of type %d\n", token_id->type);
			if(token_id->type != TYPE_IDENTIFIER) return SYNTAX_ERR;
			if(semantic_variable_input(token_id->data) == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
			return SYNTAX_OK;
			break;
		case TYPE_PRINT:
			token_expr = token_get();
			debug_print("In case print: Getting token of type %d\n", token_expr->type);
			token_save(token_expr);
			parse_expression(token_expr);
			gen_print();
			token_semicolon = token_get();
			token_save(token_semicolon);
			debug_print("In case print: Getting token of type %d\n", token_semicolon->type);
			if(token_semicolon->type != TYPE_SEMICOLON) return SYNTAX_ERR;
			if(print_n() == SYNTAX_ERR) return SYNTAX_ERR;
			return SYNTAX_OK;
			break;
		case TYPE_IF:
			token_expr = token_get();
			token_save(token_expr);
			expr_result = parse_expression(token_expr);
			if(expr_result != COMPARE_BOOL) {
				exit_program(4);
			}
			gen_if();
			
			token_then = token_get();
			token_save(token_then);
			debug_print("In case if: Getting token of type %d\n", token_then->type);
			if(token_then->type != TYPE_THEN) return SYNTAX_ERR;
			if(end() == SYNTAX_ERR) return SYNTAX_ERR;
			if(if_body(from_function, func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;

			token_else = token_get();
			token_save(token_else);
			debug_print("Getting token of type %d\n", token_else->type);
			if(token_else->type != TYPE_ELSE) return SYNTAX_ERR;
			gen_jumpif();
			if(end() == SYNTAX_ERR) return SYNTAX_ERR;
			if(if_body_end(from_function, func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;

			token_end = token_get();
			token_save(token_end);
			debug_print("Getting token of type %d\n", token_end->type);
			if(token_end->type != TYPE_END) return SYNTAX_ERR;

			token_if = token_get();
			token_save(token_if);
			debug_print("Getting token of type %d\n", token_if->type);
			if(token_if->type != TYPE_IF) return SYNTAX_ERR;
			gen_endif();
			return SYNTAX_OK;
			break;
		case TYPE_DO:
			token_while = token_get();
			token_save(token_while);
			debug_print("In case loop: Getting token of type %d\n", token_while->type);
			if(token_while->type != TYPE_WHILE) return SYNTAX_ERR;
			gen_while_label();

			token_expr = token_get();
			token_save(token_expr);
			expr_result = parse_expression(token_expr);
			if(expr_result != COMPARE_BOOL) {
				exit_program(4);
			}			
			gen_while_if();
			if(end() == SYNTAX_ERR) return SYNTAX_ERR;
			if(loop_body() == SYNTAX_ERR) return SYNTAX_ERR;

			token_loop = token_get();
			token_save(token_loop);
			debug_print("Getting token of type %d\n", token_loop->type);
			if(token_loop->type != TYPE_LOOP) return SYNTAX_ERR;
			gen_while_jump();
			return SYNTAX_OK;
			break;
		case TYPE_IDENTIFIER:
			if(semantic_variable_check(token->data) == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
			token_eq = token_get();
			token_save(token_eq);
			debug_print("In case id: Getting token of type %d\n", token_eq->type);
			if(token_eq->type != TYPE_EQUAL) return SYNTAX_ERR;
			if(right_side(token->data) == SYNTAX_ERR) return SYNTAX_ERR;
			return SYNTAX_OK;
			break;
		case TYPE_RETURN:
			if(from_function){
				*was_return = 1;
				token_ret = token_get();
				token_save(token_ret);
				debug_print("In case return: Getting token of type %d\n", token_ret->type);
				expr_type = parse_expression(token_ret);
				if(semantic_return(func_name, expr_type) == SEM_ERROR_COMP) exit_program(4);
				return SYNTAX_OK;
			}
			return SYNTAX_ERR;
			break;
		default:
			return SYNTAX_ERR;
	}
}

int print_n(){
	debug_print("********In print_n()********\n");
	if(check_epsilon(TYPE_EOL) == IS_EPSILON) return SYNTAX_OK;
	TToken *token_expr = token_get();
	token_save(token_expr);
	parse_expression(token_expr);
	gen_print();
	TToken *token_semicolon = token_get();
	token_save(token_semicolon);
	if(token_semicolon->type != TYPE_SEMICOLON) return SYNTAX_ERR;
	if(print_n() == SYNTAX_ERR) return SYNTAX_ERR;
	return SYNTAX_OK;
}

int right_side(char *var_name){
	debug_print("***Entering right_side***\n");
	TToken *token_bracket = NULL;
	TToken *token_right_bracket = NULL;
	global_item *function = NULL;
	TToken *token = token_get();
	token_save(token);

	if(token->type == TYPE_IDENTIFIER || token->type == TYPE_LENGTH || token->type == TYPE_SUBSTR || token->type == TYPE_ASC || token->type == TYPE_CHR){
		token_bracket = token_get();
		if(token_bracket->type != TYPE_BRACKET_LEFT){ //we are not calling a function but an expression
			token_unget(token_bracket);
			int expr_type = parse_expression(token);
			if(semantic_assigment(var_name ,expr_type) == SEM_ERROR_COMP) exit_program(4);
			//gen_assignment(var_name);
		}else{ //we are calling a function
			char *func_name;
			if(token->type == TYPE_LENGTH) {
				function = create_function("length");
				func_name = "length";
			}
			else if(token->type == TYPE_SUBSTR){
				function = create_function("substr");
				func_name = "substr";
			}
			else if(token->type == TYPE_ASC){
				function = create_function("asc");
				func_name = "asc";
			}
			else if(token->type == TYPE_CHR){
				function = create_function("chr");
				func_name = "chr";
			}
			else{
				function = create_function(token->data);
				func_name = token->data;
			}
			if(function == NULL){
				exit_program(99);
			}
			token_save(token_bracket);
			if(params(function) == SYNTAX_ERR) return SYNTAX_ERR;
			token_right_bracket = token_get();
			token_save(token_right_bracket);
			if(token_right_bracket->type != TYPE_BRACKET_RIGHT) return SYNTAX_ERR;
			int ret = semantic_function_call(function);
			if(ret == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
			else if(ret == SEM_ERROR_UND) exit_program(SEM_ERROR_UND);
			else if(ret == SEM_ERROR_COMP) exit_program(SEM_ERROR_COMP);
			if(semantic_function_assigment(var_name, func_name) == SEM_ERROR_COMP) exit_program(4);
		}
	} else { //the first token was number/something else
		debug_print("Sending precedence token %d\n", token->type);
		int expr_type = parse_expression(token);
		if(semantic_assigment(var_name ,expr_type) == SEM_ERROR_COMP) exit_program(4);
		//gen_assignment(var_name);
	}
	return SYNTAX_OK;
}

int params(global_item *function){
	debug_print("*******In params********\n");
	if(check_epsilon(TYPE_BRACKET_RIGHT) == IS_EPSILON) return SYNTAX_OK;
	TToken *token_id = token_get();
	token_save(token_id);
	if(token_id->type != TYPE_IDENTIFIER && token_id->type != TYPE_INTEGER_NUMBER && token_id->type != TYPE_DOUBLE_NUMBER && token_id->type != TYPE_STRING_LITERAL) return SYNTAX_ERR;
	int ret = add_called_param(function, token_id);
	if(ret == 1) exit_program(99);
	else if(ret == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
	if(expression_params_n(function) == SYNTAX_ERR) return SYNTAX_ERR;
	return SYNTAX_OK;
}

int expression_params_n(global_item *function){
	debug_print("****In expression_params_n******\n");
	if(check_epsilon(TYPE_BRACKET_RIGHT) == IS_EPSILON) return SYNTAX_OK;//EPSILON
	TToken *token_comma = token_get();
	token_save(token_comma);
	if(token_comma->type != TYPE_COMMA) return SYNTAX_ERR;
	TToken *token_id = token_get();
	token_save(token_id);
	if(token_id->type != TYPE_IDENTIFIER && token_id->type != TYPE_INTEGER_NUMBER && token_id->type != TYPE_DOUBLE_NUMBER && token_id->type != TYPE_STRING_LITERAL) return SYNTAX_ERR;
	int ret = add_called_param(function, token_id);
	if(ret == 1) exit_program(99);
	else if(ret == SEM_ERROR_REDEF) exit_program(SEM_ERROR_REDEF);
	if(expression_params_n(function) == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int functions(){
	debug_print("***Entering functions***\n");
	if(check_epsilon(TYPE_SCOPE) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(check_epsilon(TYPE_EOF) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(function() == SYNTAX_ERR) return SYNTAX_ERR;
	if(check_epsilon(TYPE_EOF) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(functions() == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int function(){
	debug_print("***Entering function***\n");
	TToken *token = token_get();
	token_save(token);
	debug_print("Getting token of type %d\n", token->type);
	TToken *token_func = NULL;
	TToken *token_id = NULL;
	TToken *token_open = NULL;
	TToken *token_close = NULL;
	TToken *token_as = NULL;
	global_item *function = NULL;
	int sem;

	switch(token->type){
		case TYPE_DECLARE:
			token_func = token_get();
			token_save(token_func);
			debug_print("In case declare: Getting token of type %d\n", token_func->type);
			if(token_func->type != TYPE_FUNCTION) return SYNTAX_ERR;
			
			token_id = token_get();
			token_save(token_id);
			debug_print("Getting token of type %d\n", token_id->type);
			if(token_id->type != TYPE_IDENTIFIER) return SYNTAX_ERR;
			function = create_function(token_id->data); 
			if(function == NULL){
				exit_program(99);
			}

			token_open = token_get();
			token_save(token_open);
			debug_print("Getting token of type %d\n", token_open->type);
			if(token_open->type != TYPE_BRACKET_LEFT) return SYNTAX_ERR;

			if(func_params(function) == SYNTAX_ERR) return SYNTAX_ERR;

			token_close = token_get();
			token_save(token_close);
			debug_print("Getting token of type %d\n", token_close->type);
			if(token_close->type != TYPE_BRACKET_RIGHT) return SYNTAX_ERR;

			token_as = token_get();
			token_save(token_as);
			debug_print("Getting token of type %d\n", token_as->type);
			if(token_as->type != TYPE_AS) return SYNTAX_ERR;

			if(func_ret_type(function) == SYNTAX_ERR) return SYNTAX_ERR;
			sem = semantic_function(function);
			if(sem == SEM_ERROR_REDEF){
				exit_program(3);
			} 
			add_function(function);
			return SYNTAX_OK;
			break;
		case TYPE_FUNCTION:
			token_id = token_get();
			token_save(token_id);
			debug_print("In case function: Getting token of type %d\n", token_id->type);
			if(token_id->type != TYPE_IDENTIFIER) return SYNTAX_ERR;
			function = create_function(token_id->data);
			if(function == NULL){
				exit_program(99);
			}
			set_function_defined(function);

			token_open = token_get();
			token_save(token_open);
			debug_print("Getting token of type %d\n", token_open->type);
			if(token_open->type != TYPE_BRACKET_LEFT) return SYNTAX_ERR;

			if(func_params(function) == SYNTAX_ERR) return SYNTAX_ERR;

			token_close = token_get();
			token_save(token_close);
			debug_print("Getting token of type %d\n", token_close->type);
			if(token_close->type != TYPE_BRACKET_RIGHT) return SYNTAX_ERR;

			token_as = token_get();
			token_save(token_as);
			debug_print("Getting token of type %d\n", token_as->type);
			if(token_as->type != TYPE_AS) return SYNTAX_ERR;

			if(func_ret_type(function) == SYNTAX_ERR) return SYNTAX_ERR;
			if(end() == SYNTAX_ERR) return SYNTAX_ERR;
			
			sem = semantic_function(function);
			if(sem == SEM_ERROR_REDEF){
				exit_program(SEM_ERROR_REDEF);
			} else if (sem == SEM_OK) {
				add_function(function);
			} 
			//gen_fucntion(function);
			if(func_scope(token_id->data) == SYNTAX_ERR) return SYNTAX_ERR;
			reset_variable_table();
			return SYNTAX_OK;
			break;
		default:
			return SYNTAX_ERR;
	}
}

int func_params(global_item *function){
	debug_print("****Entering func_params****\n");
	if(check_epsilon(TYPE_BRACKET_RIGHT) == IS_EPSILON) return SYNTAX_OK; //EPSILON

	TToken *token_id = token_get();
	token_save(token_id);
	debug_print("Getting token of type %d\n", token_id->type);
	if(token_id->type != TYPE_IDENTIFIER) return SYNTAX_ERR;
	variable_item *var = add_function_param(function, token_id->data);
	if(var == NULL){
		exit_program(99);
	}
	variable_item *var2 = create_variable(token_id->data);
	if(var == NULL){
		exit_program(99);
	}
	TToken *token_as = token_get();
	token_save(token_as);
	debug_print("Getting token of type %d\n", token_as->type);
	if(token_as->type != TYPE_AS) return SYNTAX_ERR;

	if(var_type(var) == SYNTAX_ERR) return SYNTAX_ERR;
	var2->variable_type = var->variable_type;
	add_variable(var2);
	if(func_params_n(function) == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int func_params_n(global_item *function){
	debug_print("****Entering func_params_n****\n");
	if(check_epsilon(TYPE_BRACKET_RIGHT) == IS_EPSILON) return SYNTAX_OK; //EPSILON

	TToken *token_comma = token_get();
	token_save(token_comma);
	debug_print("Getting token of type %d\n", token_comma->type);
	if(token_comma->type != TYPE_COMMA) return SYNTAX_ERR;

	TToken *token_id = token_get();
	token_save(token_id);
	debug_print("Getting token of type %d\n", token_id->type);
	if(token_id->type != TYPE_IDENTIFIER) return SYNTAX_ERR;
	variable_item *var = add_function_param(function, token_id->data);
	if(var == NULL){
		exit_program(99);
	}
	variable_item *var2 = create_variable(token_id->data);
	if(var2 == NULL){
		exit_program(99);
	}
	TToken *token_as = token_get();
	token_save(token_as);
	debug_print("Getting token of type %d\n", token_as->type);
	if(token_as->type != TYPE_AS) return SYNTAX_ERR;

	if(var_type(var) == SYNTAX_ERR) return SYNTAX_ERR;
	var2->variable_type = var->variable_type;
	add_variable(var2);
	if(func_params_n(function) == SYNTAX_ERR) return SYNTAX_ERR;
	return SYNTAX_OK;
}

int func_ret_type(global_item *function){
	debug_print("***Entering type***\n");
	TToken *token_type = token_get();
	token_save(token_type);
	debug_print("Getting token of type %d\n", token_type->type);

	switch(token_type->type){
		case TYPE_DOUBLE:
			set_function_type(function, TYPE_DOUBLE);
			return SYNTAX_OK;
			break;
		case TYPE_INTEGER:
			set_function_type(function, TYPE_INTEGER);
			return SYNTAX_OK;
			break;
		case TYPE_STRING:
			set_function_type(function, TYPE_STRING);
			return SYNTAX_OK;
			break;
		default:
			return SYNTAX_ERR;
	}
}

int var_type(variable_item *var){
	debug_print("***Entering type***\n");
	TToken *token_type = token_get();
	token_save(token_type);
	debug_print("Getting token of type %d\n", token_type->type);

	switch(token_type->type){
		case TYPE_DOUBLE:
			set_variable_type(var, TYPE_DOUBLE);
			return SYNTAX_OK;
			break;
		case TYPE_INTEGER:
			set_variable_type(var, TYPE_INTEGER);
			return SYNTAX_OK;
			break;
		case TYPE_STRING:
			set_variable_type(var, TYPE_STRING);
			return SYNTAX_OK;
			break;
		default:
			return SYNTAX_ERR;
	}
}

int func_scope(void *func_name){
	int was_return;
	was_return = 0;
	debug_print("****Entering func_scope****\n");
	if(func_body(func_name, &was_return) == SYNTAX_ERR) return SYNTAX_ERR;

	TToken *token_end = token_get();
	token_save(token_end);
	debug_print("Getting token of type %d\n", token_end->type);
	if(token_end->type != TYPE_END) return SYNTAX_ERR;

	TToken *token_function = token_get();
	token_save(token_function);
	debug_print("Getting token of type %d\n", token_function->type);
	if(token_function->type != TYPE_FUNCTION) return SYNTAX_ERR;
	if(was_return == 0){
		semantic_function_noreturn(func_name);
	}
	return SYNTAX_OK;
}

int func_body(void *func_name, int* was_return){
	debug_print("****Entering func_body****\n");
	if(check_epsilon(TYPE_END) == IS_EPSILON) return SYNTAX_OK; //EPSILON
	if(func_statement(func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;
	if(end() == SYNTAX_ERR) return SYNTAX_ERR;
	if(func_body(func_name, was_return) == SYNTAX_ERR) return SYNTAX_ERR;

	return SYNTAX_OK;
}

int func_statement(void *func_name,int* was_return){
	debug_print("***Entering func_statement***\n");
	return statement_list(1, func_name, was_return);
}