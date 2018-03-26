/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xblask04
 * Names: Barbora Blašková
 */
#ifndef HANDLER_H
#define HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "lexical_analysis.h"
#include "symtable.h"
#include "generator_stack.h"

/*
 * Structure to save incoming tokens
 */
typedef struct item_token{
	TToken *token;
	struct item_token *next_token;
}TItem_token;

/*
 * Global variable for saving tokens
 */
extern TItem_token *saved_tokens;

/*
 * Initialize token_list
 */
void init_tokenlist();
/*
 * Deinitialize token_list
 */
void deinit_tokenlist();
/*
 * Save token to the list
 */
void token_save(TToken *token);
/*
 * Deinit everything allocated
 */
void garbage_collector();


#endif