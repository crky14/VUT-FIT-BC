/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00, xblask04, xbarte14, xkalus05
 * Names: Jakub Crkoň
 */
#ifndef IFJ_BBJK__GENERATOR_H_
#define IFJ_BBJK__GENERATOR_H_

#include "lexical_analysis.h"
#include "prec_stack.h"
#include "generator_stack.h"

/**
 * @typedef Generator functions
 * @{
 */


/**
 * @brief Generate needed lines of IFJcode17 and built in functions
 */  
void gen_init();

/**
 * @brief Generate push on datastack
 * @param Token
 */  
void gen_datapush(TToken *token);

/**
 * @brief Generate code for rouding numbers form datastack
 */  
void gen_datastack_roundboth();

/**
 * @brief Retype number from datastack to double
 */  
void gen_retype_double();

/**
 * @brief Atirhmetic operation on datastack
 */ 
void gen_datastack_operation(TStack_itmtype type);

/**
 * @brief Retype first number from datastack to double
 */ 
void gen_datastack_retypefirst();

/**
 * @brief Retype second number from datastack to double
 */ 
void gen_datastack_retypesecond();

/**
 * @brief Retype first, second number from datastack to double
 */ 
void gen_datastack_retypeboth();

/**
 * @brief Generate assignment
 */ 
void gen_variable_operation(TStack_itmtype type);


/**
 * @brief Generate string concatenation
 */ 
void gen_datastack_concat();


/**
 * @brief Generate function code
 */ 
void gen_function(global_item *function);


/**
 * @brief Generate function call
 */ 
void gen_function_call(global_item *function);


/**
 * @brief Generate code for function return
 */ 
void gen_function_return();

/**
 * @brief Generate code for scope
 */ 
void gen_scope();

/**
 * @brief Generate code asiisgnment code
 */ 
void gen_assignment(char *var_name);
	
/**
 * @brief Generate code varaible declaration
 */ 
void gen_var(variable_item *item);

/**
 * @brief Generate code for if 
 */ 
void gen_if();

/**
 * @brief Generate code for if jump 
 */ 
void gen_jumpif();

/**
 * @brief Generate code for end of if
 */ 
void gen_endif();

/**
 * @brief Generate label for while
 */ 
void gen_while_label();

/**
 * @brief Generate while condition
 */ 
void gen_while_if();

/**
 * @brief Generate while jump and end
 */ 
void gen_while_jump();

/**
 * @brief Generate output
 */ 
void gen_print();

/**
 * @brief Generate input
 */ 
void gen_input(char *name, sem_types type);

/**
 * @brief Generate return of fucntion withou return
 */ 
void gen_no_return(sem_types type);

/**
 * @brief Generate createframe
 */ 
void gen_createframe();

/** @} */
#endif