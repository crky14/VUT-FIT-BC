/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00, xblask04, xbarte14, xkalus05
 * Names: Jakub Crkoň, Barbora Blašková, Bronislav Bárteček, Katarína Kalusová
 */
#include "symtable.h"

global_table *func_table;
local_table *var_table;

void gen_datapush(TToken *token);

unsigned int hash_function(const char *str) {
          unsigned int h = 0;
          const unsigned char *p;
          for(p=(const unsigned char*)str; *p!='\0'; p++){
              h = 65599*h + *p;
          }
          return (h % TABLE_SIZE);
}

int init_tables() {

	func_table = malloc(sizeof(global_table));
	if(func_table == NULL) {
		return 99;
	}

	var_table = malloc(sizeof(local_table));
	if(var_table == NULL) {
		return 99;
	}

	for (int i = 0; i < TABLE_SIZE ; i++) {
        (*func_table)[i] = NULL;
        (*var_table)[i] = NULL;
    }
	global_item *function;
	variable_item *param;
	//VSTAVANE FUNKCIE 
	
    function = create_function("length");
    set_function_type(function, TYPE_INTEGER);
    param = add_function_param(function, "s");
    set_variable_type(param, TYPE_STRING);
    set_function_defined(function);
    add_function(function);

    function = create_function("substr");
    set_function_type(function, TYPE_STRING);
    param = add_function_param(function, "s");
    set_variable_type(param, TYPE_STRING);
    param = add_function_param(function, "i");
    set_variable_type(param, TYPE_INTEGER);
    param = add_function_param(function, "n");
    set_variable_type(param, TYPE_INTEGER);
    set_function_defined(function);
    add_function(function);

    function = create_function("asc");
    set_function_type(function, TYPE_INTEGER);
    param = add_function_param(function, "s");
    set_variable_type(param, TYPE_STRING);
    param = add_function_param(function, "i");
    set_variable_type(param, TYPE_INTEGER);
    set_function_defined(function);
    add_function(function);

    function = create_function("chr");
    set_function_type(function, TYPE_STRING);
    param = add_function_param(function, "i");
    set_variable_type(param, TYPE_INTEGER);
    set_function_defined(function);
    add_function(function);

    return 1;
}

variable_item * search_variable(char *key) {
	variable_item *item = (*var_table)[hash_function(key)];
	while(item != NULL) {
		if(strcmp(item->name, key) == 0) {
			return item;
		}
		item = item->next;
	}
	return NULL;
}

global_item * search_function(char *key) {
	global_item *item = (*func_table)[hash_function(key)];
	while(item != NULL) {
		if(strcmp(item->name, key) == 0) {
			return item;
		}
		item = item->next;
	}
	return NULL;

}

global_item * create_function(char *key) {
	global_item *insert = malloc(sizeof(global_item));
	if(insert == NULL) {
		return ALLOC_FAIL;
	}
	insert->name = key;
	insert->is_defined = false;
	insert->return_type = NOT_DEFINED;
	insert->params = NULL;
	insert->params_count = 0;
	return insert;
}

void add_function (global_item *item) {
	if(item == NULL) {
		return;
	}
	if((*func_table)[hash_function(item->name)] == NULL) {
		item->next = NULL;
	}
	else{
		item->next = (*func_table)[hash_function(item->name)];
	}
	(*func_table)[hash_function(item->name)] = item;
}

void set_function_defined(global_item *item) {
	if(item == NULL) {
		return;
	}
	item->is_defined = true;
}

void set_function_type(global_item *item, TToken_type type) {
	if(item == NULL) {
		return;
	}
	if(type == TYPE_INTEGER) {
		item->return_type = INTEGER;
	}
	else if(type == TYPE_DOUBLE) {
		item->return_type = DOUBLE;
	}
	else if(type == TYPE_STRING) {
		item->return_type = STRING;
	}
}

variable_item * add_function_param(global_item *item, char *key) {
	if(strcmp(item->name, key) == 0) {
		exit_program(3);
	}
	global_item *function;
	for(int i = 0; i < (TABLE_SIZE); i++) {
    	function = (*func_table)[i];
        while(function != NULL) {
			if(strcmp(function->name, key) == 0){
				exit_program(3);
			}  
			function = function->next;
        }
    }

	variable_item *func_params = item->params;
	while(func_params != NULL) {
		if(strcmp(func_params->name, key) == 0) {
			exit_program(3);
		}
		func_params = func_params->next;
	}

	variable_item *insert = malloc(sizeof(variable_item));
	if(insert == NULL) {
		return ALLOC_FAIL;
	}

	insert->name = key;
	insert->variable_type = NOT_DEFINED;



	if(item->params == NULL) {
		insert->next = NULL;
	}
	else{
		insert->next = item->params;
	}
	item->params = insert;
	item->params_count++;
	return insert;
}

int add_called_param(global_item *item, TToken *token) {
	variable_item *insert = malloc(sizeof(variable_item));
	if(insert == NULL) {
		return 1;
	}
	if(token->type == TYPE_IDENTIFIER) {
		variable_item *var = search_variable(token->data);
		if(var == NULL) {
			return 3;
		}
		gen_datapush(token);
		insert->name = var->name;
		insert->variable_type = var->variable_type;
	}
	else{
		insert->name = NULL;
		if(token->type == TYPE_INTEGER_NUMBER) {
			gen_datapush(token);
			insert->variable_type = INTEGER;
		}
		else if(token->type == TYPE_DOUBLE_NUMBER){
			gen_datapush(token);
			insert->variable_type = DOUBLE;
		}
		else if(token->type == TYPE_STRING_LITERAL){
			gen_datapush(token);
			insert->variable_type = STRING;
		}
	}
	if(item->params == NULL) {
		insert->next = NULL;
	}
	else{
		insert->next = item->params;
	}
	item->params = insert;
	item->params_count++;
	return 0;
}


variable_item * create_variable(char *key) {
	variable_item *insert = malloc(sizeof(variable_item));
	if(insert == NULL) {
		return ALLOC_FAIL;
	}
	insert->name = key;
	insert->variable_type = NOT_DEFINED;
	return insert;
}

void add_variable(variable_item *item) {
	if(item == NULL) {
		return;
	}
	if((*var_table)[hash_function(item->name)] == NULL) {
		item->next = NULL;
	}
	else{
		item->next = (*var_table)[hash_function(item->name)];
	}
	(*var_table)[hash_function(item->name)] = item;
}

void set_variable_type(variable_item *item, TToken_type type) {
	if(item == NULL) {
		return;
	}
	if(type == TYPE_INTEGER) {
		item->variable_type = INTEGER;
	}
	else if(type == TYPE_DOUBLE) {
		item->variable_type = DOUBLE;
	}
	else if(type == TYPE_STRING) {
		item->variable_type = STRING;
	}
}


void delete_function(global_item *delete) {
	if(delete == NULL) {
		return;
	}
	while(delete->params != NULL) {
		variable_item *tmp = delete->params;
		delete->params = delete->params->next;
		free(tmp);
	}
	free(delete);
}

void delete_variable(variable_item *delete) {
	if(delete != NULL){
		free(delete);
	}
}

void reset_variable_table() {
	variable_item *delete_var;
	for(int i = 0; i < (TABLE_SIZE); i++) {
    	//uvolnenie lokalnej tabulky
        while((*var_table)[i] != NULL) {
            delete_var = (*var_table)[i];                   
            (*var_table)[i] = (*var_table)[i]->next; 
            free(delete_var);    
        }
        (*var_table)[i] = NULL;
    }
}


void dispose_tables() {
	global_item *delete_func;
	variable_item *delete_var;

    for(int i = 0; i < (TABLE_SIZE); i++) {
    	//uvolnenie lokalnej tabulky
        while((*var_table)[i] != NULL) {
            delete_var = (*var_table)[i];                  
            (*var_table)[i] = (*var_table)[i]->next; 
            free(delete_var);    
        }
        //uvolnenie globalnej tabulky
        while((*func_table)[i] != NULL) {
            delete_func = (*func_table)[i];
            // ubvolnenie jednotlivych parametrov funkcii
            while(delete_func->params != NULL) {
            	delete_var = delete_func->params;
            	delete_func->params = delete_func->params->next;
            	free(delete_var);
            }            
            (*func_table)[i] = (*func_table)[i]->next; 
            free(delete_func);    
        }
    }
    free(var_table);
    free(func_table);
}




