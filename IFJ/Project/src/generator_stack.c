/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00, xblask04, xbarte14, xkalus05
 * Names: Jakub Crkoň
 */

#include "generator_stack.h"

TGenStack *while_stack;
TGenStack *if_stack;

int Gstack_init() {
	while_stack = malloc(sizeof(TGenStack));
	if (while_stack == NULL) {
		return 99;
	}
	if_stack = malloc(sizeof(TGenStack));
	if (while_stack == NULL) {
		return 99;
	}
	while_stack->First = NULL;
	if_stack->First = NULL;
	return 0;
}

int Gstack_push(TGenStack *S, int data) {
	TGenStack_itmptr insert = malloc(sizeof(struct TGenStack_itm));
	if(insert == NULL) {
		return 99;
	}

	insert->data= data;

	if(S->First == NULL) {
		S->First = insert;
		S->First->next = NULL;
	}
	else {
		TGenStack_itmptr tmp = S->First;
		S->First = insert;
		S->First->next = tmp;
	}
	return 1;
}

int Gstack_top(TGenStack *S) {
	if(S->First == NULL) {
		return -1;
	}
	return S->First->data;
}

void Gstack_pop(TGenStack *S) {
	if(S->First == NULL) {
		return;
	}
	TGenStack_itmptr delete = S->First;		
	S->First = S->First->next;
	free(delete);
}


void Gstack_dispose() {
	TGenStack_itmptr tmp;
	while(while_stack->First != NULL) {
		tmp = while_stack->First;
		while_stack->First = while_stack->First->next;
		free(tmp);
	}
	while(if_stack->First != NULL) {
		tmp = if_stack->First;
		if_stack->First = if_stack->First->next;
		free(tmp);
	}
	free(while_stack);
	free(if_stack);
}