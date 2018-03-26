/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#ifndef IFJ_BBJK__PRECEDENCE_ANALYSIS_H
#define IFJ_BBJK__PRECEDENCE_ANALYSIS_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexical_analysis.h"
#include "exit.h"
#include "token_handler.h"
#include "prec_stack.h"
#include "generator.h"

#define TABLE_ROWS 8
#define TABLE_COLS 10
#define MAX_RULE_LENGTH 3

/**
 * @typedef hashtable structures
 * \@{
 */
typedef enum {
/* 1  */	S,	/* SHIFT */
/* 2  */	R,	/* REDUCE */
/* 3  */	H, 	/* HANDLE */
/* 4  */	E,	/* ERROR */
/* 5  */	F   /* FINISH */
} prec_operations;
/** @} */

/**
 * @typedef Precedence table
 * \@{
 */
extern const int precedence_table[TABLE_ROWS][TABLE_COLS];
/** @} */

/**
 * @brief Parse expressio 
 * @param First token of epression
 * @return semantic type of expression
 */
int parse_expression(TToken *token);

/**
 * @brief Check table
 * @param Pointer to precedence stack
 * @param Current token
 * @return Operation which will be terminated
 */
prec_operations check_table(TStack *stack, TToken *token);

/**
 * @brief Change token type to precedence type
 * @param Token to be changed
 * @return Precedence stack type 
 */
TStack_itmtype map_token(TToken *token);

/**
 * @brief Check if rule to reduce exists and execute it 
 * @param Pointer to precedence stack
 * @param Current token
 * @param Rule length
 */
void check_rules(TStack *stack, TStack_itmptr *rule, int rule_len);

/**
 * @brief Pops rule members from stack
 * @param Pointer to precedence stack
 * @param Length of rule
 */
void execute_rule(TStack *stack, int rule_len);

#endif
