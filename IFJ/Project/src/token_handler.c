/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xblask04
 * Names: Barbora Blašková
 */
#include "token_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include "lexical_analysis.h"
#include "symtable.h"


TItem_token *saved_tokens;

void init_tokenlist(){
	saved_tokens = NULL;
}

void deinit_tokenlist(){
	while(saved_tokens != NULL){
		TItem_token *tmp = saved_tokens->next_token;
		token_free(saved_tokens->token);
		free(saved_tokens);
		saved_tokens = tmp;
	}
}

void token_save(TToken *token){
	TItem_token *new_item = (TItem_token*)malloc(sizeof(struct item_token));
	if(new_item == NULL){
		garbage_collector();
		exit(99);
	}
	new_item->token = token;
	new_item->next_token = saved_tokens;
	saved_tokens = new_item;
}

void garbage_collector(){
	deinit_tokenlist();
	dispose_tables();
	Gstack_dispose();
}
