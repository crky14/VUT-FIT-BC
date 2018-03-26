/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00
 * Names: Jakub Crkoň
 */

#include "semantic.h"
#include "prec_stack.h"


int semantic_function(global_item *insert) {
	global_item *exist = search_function(insert->name);
	if(exist != NULL) {
		if(exist->is_defined == false && insert->is_defined == true) {
			if(insert->return_type != exist->return_type) {
				delete_function(insert);
				insert = NULL;
				return SEM_ERROR_REDEF;
			}
			if(insert->params_count != exist->params_count) {
				delete_function(insert);
				insert = NULL;
				return SEM_ERROR_REDEF;
			}
			variable_item *insert_param = insert->params;
			variable_item *exist_param = exist->params;
			while(insert_param != NULL) {
				if(insert_param->variable_type != exist_param->variable_type) {
					delete_function(insert);
					insert = NULL;
					return SEM_ERROR_REDEF;
				}
				exist_param = exist_param->next;
				insert_param = insert_param->next;
			}
			set_function_defined(exist);
			gen_function(exist);
			delete_function(insert);
			insert = NULL;
			return SEM_DEFINED;

		}
		else if(exist->is_defined == true){
			delete_function(insert);
			insert = NULL;
			return SEM_ERROR_REDEF;
		}
	}
	else {
		if(insert->is_defined == true)
			gen_function(insert);
	}

	return SEM_OK;
}

int semantic_variable(variable_item *variable) {
	if(search_variable(variable->name) == NULL && search_function(variable->name) == NULL) {
		return SEM_OK;
	}
	delete_variable(variable);
	variable = NULL;
	return SEM_ERROR_REDEF;
}

int semantic_variable_check(char *name) {
	if(search_variable(name) != NULL) {
		return SEM_OK;
	}
	return SEM_ERROR_REDEF;
}

int semantic_variable_input(char *name) {
	variable_item *item;
	if((item = search_variable(name)) != NULL) {
		gen_input(name, item->variable_type);
		return SEM_OK;
	}
	return SEM_ERROR_REDEF;
}



int semantic_function_call(global_item *insert) {
	global_item *function = search_function(insert->name);
	if(function == NULL) {
		delete_function(insert);
		return SEM_ERROR_REDEF;
	}
	if(insert->params_count != function->params_count) {
		delete_function(insert);
		return SEM_ERROR_COMP;
	}
	variable_item *vars = insert->params;
	variable_item *params = function->params;
	while(vars != NULL) {
		if(vars->name != NULL) {
			variable_item *variable = search_variable(vars->name);
			if(variable == NULL) {
				delete_function(insert);
				return SEM_ERROR_REDEF;
			}
			if(variable->variable_type != params->variable_type) {
				delete_function(insert);
				return SEM_ERROR_COMP;
			}
		}
		else {
			if(vars->variable_type != params->variable_type) {
				delete_function(insert);
				return SEM_ERROR_COMP;
			}
		}
		vars = vars->next;
		params = params->next;
	}
	gen_function_call(insert);
	delete_function(insert);
	return SEM_OK;
}

int semantic_identifier(char *name) {
	variable_item *item = search_variable(name);
	if(item == NULL) {
		return NOT_DEFINED;
	}
	return item->variable_type;
} 

int semantic_expression(TStack_itmptr *operands) {

	if(operands[0]->sem_type == STRING && operands[2]->sem_type == STRING && operands[1]->type != S_MINUS && operands[1]->type != S_MULTI && operands[1]->type != S_DIV && operands[1]->type != S_INT_DIV) {
		if(operands[1]->type == S_PLUS) {
			gen_datastack_concat();
		}
		else if(operands[1]->type == S_EQUAL || operands[1]->type == S_NOT_EQUAL || operands[1]->type == S_MORE || operands[1]->type == S_LESS || operands[1]->type == S_MORE_EQUAL || operands[1]->type == S_LESS_EQUAL) {
			gen_datastack_operation(operands[1]->type);
			return  COMPARE_BOOL;
		}
		return STRING;
	}
	else if(operands[0]->sem_type == DOUBLE || operands[2]->sem_type == DOUBLE) {
		if(operands[0]->sem_type == INTEGER) {
			gen_datastack_retypefirst(0);
			gen_datastack_operation(operands[1]->type);
		}
		else if (operands[2]->sem_type == INTEGER){
			gen_datastack_retypesecond();
			gen_variable_operation(operands[1]->type);
		}
		else {
			gen_datastack_operation(operands[1]->type);
		}
		if(operands[1]->type == S_INT_DIV) {
				return INTEGER;
		}
		else if(operands[1]->type == S_EQUAL || operands[1]->type == S_NOT_EQUAL || operands[1]->type == S_MORE || operands[1]->type == S_LESS || operands[1]->type == S_MORE_EQUAL || operands[1]->type == S_LESS_EQUAL) {
			return COMPARE_BOOL;
		}
		return DOUBLE;
	}
	else if(operands[0]->sem_type == INTEGER && operands[2]->sem_type == INTEGER) {
		if(operands[1]->type == S_INT_DIV || operands[1]->type == S_DIV) {
			gen_datastack_retypeboth();
			gen_variable_operation(operands[1]->type);
			if(operands[1]->type == S_DIV) {
				return DOUBLE;
			}
		}
		else {
			gen_datastack_operation(operands[1]->type);
		}
		if(operands[1]->type == S_EQUAL || operands[1]->type == S_NOT_EQUAL || operands[1]->type == S_MORE || operands[1]->type == S_LESS || operands[1]->type == S_MORE_EQUAL || operands[1]->type == S_LESS_EQUAL) {
			return COMPARE_BOOL;
		}
		return INTEGER;
	}
	return NOT_DEFINED;
}

int semantic_assigment(char *name, sem_types expr_type) {
	variable_item *item = search_variable(name);
	if(item == NULL) {
		return SEM_ERROR_REDEF;
	}
	if(item->variable_type == DOUBLE && expr_type == INTEGER) {
		gen_datastack_retypefirst(); //konvertuje vysledok na doouble z INT
		gen_assignment(name);
		return SEM_OK;
	}
	else if(item->variable_type == INTEGER && expr_type == DOUBLE) {
		gen_retype_double();
		gen_assignment(name);
		return SEM_OK;
	}	
	else if(item->variable_type == expr_type) {
		gen_assignment(name);
		return SEM_OK;
	}
	return SEM_ERROR_COMP;
}



int semantic_return(char *func_name, sem_types expr_type) {
	global_item *function = search_function(func_name);
	if(function == NULL) {
		return SEM_ERROR_REDEF;
	}
	if(expr_type == DOUBLE && function->return_type == INTEGER) {
		gen_retype_double();
		gen_function_return();
		return SEM_OK;
	}
	else if(expr_type == INTEGER && function->return_type == DOUBLE) {
		gen_datastack_retypefirst();
		gen_function_return();
		return SEM_OK;
	}	
	else if(expr_type == function->return_type) {
		gen_function_return();
		return SEM_OK;
	}
	return SEM_ERROR_COMP;
}

int semantic_function_assigment(char *var_name, char *func_name) {
	variable_item *variable = search_variable(var_name);
	global_item *function = search_function(func_name);
	if(variable->variable_type == DOUBLE && function->return_type == INTEGER) {
		gen_datastack_retypefirst(); 
		gen_assignment(var_name);
		return SEM_OK;
	}
	else if(variable->variable_type == INTEGER && function->return_type == DOUBLE) {
		gen_retype_double();
		gen_assignment(var_name);
		return SEM_OK;
	}	
	else if(variable->variable_type == function->return_type) {
		gen_assignment(var_name);
		return SEM_OK;
	}
	return SEM_ERROR_COMP;
}


int semantic_function_all() {
	for(int i = 0; i < (TABLE_SIZE); i++) {
		global_item *item = (*func_table)[i];
        while(item != NULL) {
            if(item->is_defined != true) {
            	return SEM_ERROR_REDEF;
            } 
            item = item->next;  
        }
    }
    return SEM_OK;
}

void semantic_function_noreturn(char *name) {
	global_item *function = search_function(name);
	if(function->return_type == STRING) {
		gen_no_return(STRING);
	}
	else if(function->return_type == INTEGER) {
		gen_no_return(INTEGER);
	}
	else if(function->return_type == DOUBLE) {
		gen_no_return(DOUBLE);
	}
}