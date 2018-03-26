/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#include "prec_stack.h"
#include <stdio.h>
#include <stdlib.h>


void stack_init(TStack *S) {
	S->First = NULL;
	S->count = 0;

}

void stack_pop(TStack *S) {

	if(S->count == 0) {
		return ;
	}
	TStack_itmptr delete = S->First;		
	S->First = S->First->next;
	free(delete);
	S->count--;	
}

int stack_push (TStack *S, TStack_itmtype type, sem_types sem_type) {
	TStack_itmptr insert = malloc(sizeof(struct TStack_itm));
	if(insert == NULL) {
		return 99;
	}

	insert->type = type;
	insert->sem_type = sem_type;

	if(S->First == NULL) {
		S->First = insert;
		S->First->next = NULL;
	}
	else {
		TStack_itmptr tmp = S->First;
		S->First = insert;
		S->First->next = tmp;
	}
	S->count++;
	return 1;
}


void stack_dispose (TStack *S ) {
	TStack_itmptr tmp;
	while(S->First != NULL) {
		tmp = S->First;
		S->First = S->First->next;
		free(tmp);
	}
}

TStack_itmtype stack_first_term(TStack *S) {
	TStack_itmptr tmp = S->First;
	while(tmp != NULL) {
		if(tmp->type != S_EXPR && tmp->type != S_TOP) {
			return tmp->type;
		}
		tmp = tmp->next;
	}
	return S_ERROR;
}

int stack_newtop(TStack *S) {
	TStack_itmptr tmp_prev;
	TStack_itmptr tmp = S->First;
	while(tmp != NULL) {
		if(tmp->type != S_EXPR && tmp->type != S_TOP) {
			break;
		}
		tmp_prev = tmp;
		tmp = tmp->next;
	}
	if(tmp != NULL) {
		TStack_itmptr insert = malloc(sizeof(struct TStack_itm));
		if(insert == NULL) {
			return 99;
		}
		insert->type = S_TOP;
		insert->sem_type = NOT_DEFINED;
		if(tmp == S->First) {
			S->First = insert;
			insert->next = tmp;
		}
		else{
			tmp_prev->next = insert;
			insert->next = tmp;
		}
		S->count++;
	}
	return 1;
}
