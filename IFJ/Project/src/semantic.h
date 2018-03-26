/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#ifndef IFJ_BBJK__SEMANTIC_ANALYSIS_H_
#define IFJ_BBJK__SEMANTIC_ANALYSIS_H_


#include "symtable.h"
#include "prec_stack.h"
#include "generator.h"


/**
 * @typedef Semantic errors
 * @{
 */
#define SEM_OK 0
#define SEM_DEFINED 1
#define SEM_ERROR_REDEF 3
#define SEM_ERROR_COMP	4
#define SEM_ERROR_UND	6
/** @} */

/**
 * @typedef Semantic functions
 * \@{
 */

/**
 * @brief Check function for semantic errors while function is defined or declared
 * @param Function to check
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_function(global_item *insert);

/**
 * @brief Check variable for semantic errors
 * @param Viariable to check
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_variable(variable_item *variable);

/**
 * @brief Check variable for semantic errors
 * @param Name of variable to check
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_variable_check(char *name);

/**
 * @brief Check variable for semantic errors adn generate error
 * @param Name of variable to check
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_variable_input(char *name);

/**
 * @brief Check function for semantic errors while function is called
 * @param Function to check
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_function_call(global_item *insert);

/**
 * @brief Check if variable was declared - used in precedence analysis
 * @param Name of variable
 * @return Semantic type of variable
 */
int semantic_identifier(char *name);

/**
 * @brief Check semantic types of members and calling needed generation functions - used in precedence analysis
 * @param Array of operands
 * @return Semantic type of result expression
 */
int semantic_expression(TStack_itmptr *operands);

/**
 * @brief Check if variable and expr semantic types are compatible
 * @param Variable to check
 * @param Expression type
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_assigment(char *variable_name, sem_types epxr_sem);

/**
 * @brief Check if return type of function and expr semantic types are compatible
 * @param Function to check
 * @param Expression type
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_return(char *func_name, sem_types expr_type);

/**
 * @brief Check if return type of function and variable semantic types are compatible
 * @param Variable to check
 * @param Function to check
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_function_assigment(char *var_name, char *func_name);

/**
 * @brief Check if all function are defined before main
 * @return SEM_OK on succes , one of erros on fail
 */
int semantic_function_all();

/**
 * @brief If function have no return generate needed code according to function return type
 * @return SEM_OK on succes , one of erros on fail
 */
void semantic_function_noreturn(char *name);

/** @} */
#endif
