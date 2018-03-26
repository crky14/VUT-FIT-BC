/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xblask04
 * Names: Barbora Blašková
 */
#ifndef PARSER_H
#define PARSER_H


#include <stdio.h>
#include <stdlib.h>
#include "lexical_analysis.h"
#include "symtable.h"
#include "token_handler.h"
#include "generator.h"

#define SYNTAX_ERR  2 //syntax error number
#define SYNTAX_OK   0 //syntax is ok
#define IS_EPSILON  1
#define NOT_EPSILON 0
#define PREC_ERR    0
#define PREC_OK     1
/*
 * Function will check whether the epsilon rule can not be applied
 * @param exp_type which type of token we are expecting in case of epsilon rule
 */
int check_epsilon(TToken_type exp_type);
/*
 * All the functions under are simulating tree according to the grammar
 */
int syntax_check();
int program();
int functions();
int function();
int scope();
int scope_content();
int declarations();
int declaration();
int print_n();
int assignment(void *var_name);
int end();
int end_n();
int body();
int statement();
int statement_list(int from_function, void *func_name, int* was_return);
int if_body(int from_function, void *func_name, int* was_return);
int if_body_end(int from_function, void *func_name, int* was_return);
int loop_body();
int right_side(char *var_name);
int params(global_item *function);
int expression_params_n(global_item *function);
int func_declaration();
int func_params(global_item *function);
int func_params_n(global_item *function);
int var_type(variable_item *var);
int func_ret_type(global_item *function);
int func_definition();
int func_scope(void *func_name);
int func_body(void *func_name, int* was_return);
int func_statement(void *func_name, int* was_return);

#endif
