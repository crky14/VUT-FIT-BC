/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#ifndef IFJ_BBJK__GENERATOR_STACK_H
#define IFJ_BBJK__GENERATOR_STACK_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @typedef Generator stack structures
 * @{
 */


/**
 * @brief Member of generator stack
 */ 
typedef struct TGenStack_itm {             
		int data;
        struct TGenStack_itm *next;       
} *TGenStack_itmptr;

/**
 * @brief Generator stack head
 */ 
typedef struct {                    
    TGenStack_itmptr First;                         
} TGenStack;
/** @} */

/**
 * @typedef Stack for generating nested while
 * \@{
 */
extern TGenStack *while_stack;
/** @} */
/**
 * @typedef Stack for generating nested if
 * \@{
 */
extern TGenStack *if_stack;
/** @} */

/**
 * @typedef generator sack functions
 * \@{
 */ 

/**
 * @brief Initiate both generator stacks
 * @return 0 on succes,  99 alloc fail
 */       
int Gstack_init();

/**
 * @brief Push member on stack
 * @param Pointer to stack
 * @param data for new member
 * @return 0 on succes,  99 alloc fail
 */     
int Gstack_push(TGenStack *S, int data);

/**
 * @brief Push member on stack
 * @param Pointer to stack
 * @return data from stack top
 */     
int Gstack_top(TGenStack *S);

/**
 * @brief Pop member from stack
 * @param Pointer to stack
 */       
void Gstack_pop(TGenStack *S);

/**
 * @brief Dispose stack both stacks
 */  
void Gstack_dispose();
/** @} */


#endif