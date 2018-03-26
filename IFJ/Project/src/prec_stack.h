/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#ifndef IFJ_BBJK__PRECEDENCE_STACK_H
#define IFJ_BBJK__PRECEDENCE_STACK_H

#include "lexical_analysis.h"
#include "symtable.h"

typedef enum {
	/* 1  */	S_PLUS,
	/* 2  */	S_MINUS,
	/* 3  */	S_MULTI,
	/* 4  */	S_DIV,
	/* 5  */	S_INT_DIV,
	/* 6  */	S_ID,
	/* 7  */	S_BRACELET_LEFT,
	/* 8  */	S_BRACELET_RIGHT,
	/* 9  */	S_EQUAL,
	/* 10  */	S_NOT_EQUAL,
	/* 11  */	S_MORE,
	/* 12  */	S_LESS, 
	/* 13  */	S_MORE_EQUAL, 
	/* 14  */	S_LESS_EQUAL, 
	/* 15  */	S_EOL, 
	/* 16  */	S_EXPR, 
	/* 17  */	S_TOP, 
	/* 18  */	S_ERROR 
} TStack_itmtype;

 /**
 * @typedef precedence stack structures
 * \@{
 */  

/**
 * @brief Member of precedence stack
 */ 
typedef struct TStack_itm {             
        TStack_itmtype type; 
        sem_types sem_type;	 
        struct TStack_itm *next;       
} *TStack_itmptr;

/**
 * @brief Precedence stack head
 */ 
typedef struct {                    
    TStack_itmptr First;
    int count;                             
} TStack;
/** @} */

/**
 * @typedef precedence stack functions
 * \@{
 */       

/**
 * @brief Initiate stack
 * @param Pointer to stack head
 */          
void stack_init (TStack *S);

/**
 * @brief Pop member from stack top
 * @param Pointer to stack head
 */    
void stack_pop (TStack *S);

/**
 * @brief Push member to stack
 * @param Pointer to stack head
 * @param stack type of token 
 * @param semantic type if token
 */ 
int stack_push (TStack *S, TStack_itmtype type, sem_types sem_type);

/**
 * @brief Dispose stack
 * @param Pointer to stack head
 */    
void stack_dispose (TStack *S);

/**
 * @brief Return first terminal from stack
 * @param Pointer to stack head
 * @preturn Terminal type
 */    
TStack_itmtype stack_first_term(TStack *S);

/**
 * @brief Insert S_TOP after first terminal on stack
 * @param Pointer to stack head
 * @preturn 1 on succes, 99 on alloc fail
 */ 
int stack_newtop(TStack *S);
/** @} */

#endif