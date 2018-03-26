/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00, xkalus05
 * Names: Jakub Crkoň, Katarína Kalusová
 */

/**
 * @symtable
 * Symtable documentation
 */

#ifndef IFJ_BBJK__SYM_TABLES_H_
#define IFJ_BBJK__SYM_TABLES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "buffer.h"
#include "lexical_analysis.h"

#define TABLE_SIZE 200
#define ALLOC_FAIL NULL

/**
 * @typedef semantic analysis enums
 * @{
 */

/**
 * Return types of sematic functions
 */
typedef enum {
	/* 0 */ INTEGER,
	/* 2 */	DOUBLE,
	/* 3 */	STRING,
	/* 4 */	COMPARE_BOOL,
	/* 5 */	NOT_DEFINED
} sem_types;
/** @} */

/**
 * @typedef semantic analysis structures
 * \@{
 */

/**
 * @brief Member of local symbol table for variables.
 */
typedef struct variable_item {
	char *name;					/* name of the element  */
	sem_types variable_type;	/* semantic type of the element  */
	struct variable_item *next;
} variable_item;

/**
 * @brief Member of global symbol table for functions.
 */
typedef struct global_item {
	char *name;						/* name of the element  */
	bool is_defined;				/* flag if function was defined  */
	sem_types return_type;			/* semantic type of the element  */
	struct variable_item *params;	/* pointer to params of function  */
	int params_count;				/* count of params   */
	struct global_item *next;
} global_item;

/**
 * @brief Global symbol table.
 *
 *	Global symbol for table functions implemented as hash-table
 */
typedef global_item* global_table[TABLE_SIZE];

/**
 * @brief Local symbol table.
 *
 *	Local symbol table for variables implemented as hash-table
 */
typedef variable_item* local_table[TABLE_SIZE];
/** @} */

/**
 * @typedef Global symbol table for functions
 * \@{
 */
extern global_table *func_table;
/** @} */

/**
 * @typedef Local symbol table for variables
 * \@{
 */
extern local_table *var_table;
/** @} */

/**
 * @typedef hashtable functions
 * \@{
 */

/**
 * @brief Assigns index in hashtable to the key
 * @param key of the item
 * @return index in the table
 */
unsigned int hash_function(const char *str);

/**
 * @brief Initialize both tables
 */
int init_tables();

/**
 * @brief Search variable in local symtable
 * @param Name of variable
 * @return Pointer to found variable or NULL
 */
variable_item * search_variable(char *key);

/**
 * @brief Search function in local symtable
 * @param Name of function
 * @return Pointer to found function or NULL
 */
global_item * search_function(char *key);

/**
 * @brief Create empty function 
 * @param Name of function
 * @return Pointer to created function or NULL
 */
global_item * create_function(char *key);

/**
 * @brief Insert function to symbol table
 * @param Pointer to function struct
 * @return 
 */
void add_function(global_item *item);

/**
 * @brief Set function flag is_defined to true
 * @param Pointer to function struct 
 */
void set_function_defined(global_item *item);

/**
 * @brief Set function semantic type
 * @param Pointer to function struct
 * @param Semantic type
 */
void set_function_type(global_item *item, TToken_type type);

/**
 * @brief Add function param
 * @param Name of param
 * @return pointer to inserted param
 */
variable_item * add_function_param(global_item *item, char *key);

/**
 * @brief Add function param
 * @param Pointer to function
 * @param Token of variable
 * @return 0 on succes , 1 on alloc fail, 3 on semantic fail
 */
int add_called_param(global_item *item, TToken *token);

/**
 * @brief Create empty fvariable with inserted name
 * @param Name of variable
 * @return Pointer to created function or NULL
 */
variable_item * create_variable(char *key);

/**
 * @brief Insert variable to symbol table
 * @param Pointer to variable struct
 * @return 
 */
void add_variable(variable_item *item);

/**
 * @brief Set variable semantic type
 * @param Pointer to variable struct
 * @param Token to variable
 */
void set_variable_type(variable_item *item, TToken_type type);

/**
 * @brief Free created function which is not supposed to be inserted
 * @param Pointer to function struct
 */
void delete_function(global_item *delete);

/**
 * @brief Free created variable which is not supposed to be inserted
 * @param Pointer to variable struct
 */
void delete_variable(variable_item *delete);

/**
 * @brief Reset variable table
 */
void reset_variable_table();

/**
 * @brief Dispose both tables
 */
void dispose_tables();

/** @} */
#endif