/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: BBJK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00, xblask04, xbarte14, xkalus05
 * Names: Jakub Crkoň
 */
#include "generator.h"

int tmp_count;

void gen_init() {
	printf(".IFJcode17\nJUMP $scope\n");


	printf("LABEL $length\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n"

			"DEFVAR LF@string_length0\n"
			"DEFVAR LF@count_length0\n"
			"POPS LF@string_length0\n"
			"STRLEN LF@count_length0 LF@string_length0\n"
			"PUSHS LF@count_length0\n"

			"POPFRAME\n"
			"RETURN\n");

	printf("LABEL $substr\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n"

			"DEFVAR LF@string_substr0\n"
			"MOVE LF@string_substr0 string@\n"
			"DEFVAR LF@position_substr0\n"
			"MOVE LF@position_substr0 int@0\n"
			"DEFVAR LF@length_substr0\n"
			"MOVE LF@length_substr0 int@0\n"
			"DEFVAR LF@final_string_substr0\n"
			"MOVE LF@final_string_substr0 string@\n"
			"DEFVAR LF@counter_substr0\n"
			"MOVE LF@counter_substr0 int@0\n"
			"DEFVAR LF@string_substr1\n"
			"MOVE LF@string_substr1 string@\n"
			"DEFVAR LF@bool_substr0\n"
			"MOVE LF@bool_substr0 bool@FALSE\n"
			"DEFVAR LF@string_length_substr0\n"
			"MOVE LF@string_length_substr0 int@0\n"
			"DEFVAR LF@some_integer_substr0\n"
			"MOVE LF@some_integer_substr0 int@0\n"
			"POPS LF@length_substr0\n"
			"POPS LF@position_substr0\n"
			"POPS LF@string_substr0\n"
			"JUMPIFEQ Error_substr LF@string_substr0 string@\n"
			"LT LF@bool_substr0 int@0 LF@position_substr0\n"
			"JUMPIFNEQ Error_substr LF@bool_substr0 bool@TRUE\n"
			"LT LF@bool_substr0 int@0 LF@length_substr0\n"
			"JUMPIFNEQ Error_substr LF@bool_substr0 bool@TRUE\n"
			"STRLEN LF@string_length_substr0 LF@string_substr0\n"
			"GT LF@bool_substr0 LF@position_substr0 LF@string_length_substr0\n"
			"JUMPIFEQ Error_substr LF@bool_substr0 bool@TRUE\n"
			"ADD LF@some_integer_substr0 LF@length_substr0 LF@position_substr0\n"
			"SUB LF@some_integer_substr0 LF@some_integer_substr0 int@1\n"
			"GT LF@bool_substr0 LF@some_integer_substr0 LF@string_length_substr0\n"
			"JUMPIFNEQ Loop_substr0 LF@bool_substr0 bool@TRUE\n"
			"SUB LF@length_substr0 LF@some_integer_substr0 LF@position_substr0\n"
			"MOVE LF@counter_substr0 LF@position_substr0\n"
			"MOVE LF@final_string_substr0 string@\n"
			"SUB LF@counter_substr0 LF@counter_substr0 int@1\n"
			"LABEL Loop_substr0\n"
			"GETCHAR LF@string_substr1 LF@string_substr0 LF@counter_substr0\n"
			"CONCAT LF@final_string_substr0 LF@final_string_substr0 LF@string_substr1\n"
			"SUB LF@length_substr0 LF@length_substr0 int@1\n"
			"ADD LF@counter_substr0 LF@counter_substr0 int@1\n"
			"EQ LF@bool_substr0 LF@length_substr0 int@0\n"
			"JUMPIFEQ Loop_substr0 LF@bool_substr0 bool@FALSE\n"
			"PUSHS LF@final_string_substr0\n"
			"JUMP End_substr\n"
			"LABEL Error_substr\n"
			"PUSHS string@\n"
			"LABEL End_substr\n"

			"POPFRAME\n"
			"RETURN\n");

	printf("LABEL $asc\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n"

			"DEFVAR LF@string_asc0\n"
			"DEFVAR LF@char_asc0\n"
			"DEFVAR LF@position_asc0\n"
			"DEFVAR LF@string_count_asc0\n"
			"DEFVAR LF@bool_asc0\n"
			"POPS LF@position_asc0\n"
			"POPS LF@string_asc0\n"
			"STRLEN LF@string_count_asc0 LF@string_asc0\n"
			"LT LF@bool_asc0 LF@position_asc0 LF@string_count_asc0\n"
			"JUMPIFNEQ Error_asc bool@TRUE LF@bool_asc0\n"
			"GT LF@bool_asc0 LF@position_asc0 int@0\n"
			"JUMPIFNEQ Error_asc bool@TRUE LF@bool_asc0\n"
			"SUB LF@position_asc0 LF@position_asc0 int@1\n"
			"STRI2INT LF@char_asc0 LF@string_asc0 LF@position_asc0 \n"
			"PUSHS LF@char_asc0\n"
			"JUMP End_asc\n"
			"LABEL Error_asc\n"
			"PUSHS int@0\n"
			"LABEL End_asc\n"
	
			"POPFRAME\n"
			"RETURN\n");

	printf("LABEL $chr\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n"

			"DEFVAR LF@value_chr0\n"
			"DEFVAR LF@string_chr0\n"
			"POPS LF@value_chr0\n"
			"INT2CHAR LF@string_chr0 LF@value_chr0\n"
			"PUSHS LF@string_chr0\n"

			"POPFRAME\n"
			"RETURN\n");

}

void gen_datapush(TToken *token) {
	if(token->type == TYPE_IDENTIFIER) {
		printf("PUSHS LF@%s\n", (char*)token->data);
	}
	else if(token->type == TYPE_INTEGER_NUMBER){
		printf("PUSHS int@%d\n", *((int*)token->data));
	}
	else if(token->type == TYPE_DOUBLE_NUMBER){
		printf("PUSHS float@%g\n", *((double*)token->data));
	}
	else if(token->type == TYPE_STRING_LITERAL) {
		char *string = (char*)token->data;
		printf("PUSHS string@");
		int i = 0;
		while(string[i] != '\0'){
			if(string[i] <= 32 || string[i] == 35){
				printf("\\0%d", string[i]);
			}
			else if (string[i] == 92){
				i = i +1;
				if(string[i] == '\0') exit_program(1);
				else if(string[i] == 92) printf("\\092");
				else if (string[i] == 'n') printf("\\010");
				else if (string[i] == 't') printf("\\009");
				else if (string[i] == 34) printf("\\034");
				else if (isdigit(string[i])){
					int j = string[i]-48;
					i = i + 1;
					if(string[i] == '\0') exit_program(1);
					if (isdigit(string[i])){
						j = (j*10 ) + (string[i]-48);
					}
					else{
						exit_program(1);
					}
					i = i+1;
					if(string[i] == '\0') exit_program(1);
					if (isdigit(string[i])){
						j = (j*10 ) + (string[i]-48);
						if ((j>255) || (j<=0)){
							exit_program(1);
						}
					}
					else{
						exit_program(1);
					}
					printf("\\%03d",j);
				}
				else {
					exit_program(1);
				}
			}
			else {
				printf("%c", string[i]);
			}
			i = i+1;
		}
		printf("\n");
	}
}

void gen_datastack_roundboth() {
	static int round_counter = 0;

	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("MOVE TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-1);
	printf("FLOAT2INT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("DIV TF@tmp%d TF@tmp%d float@2.0\n", tmp_count, tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("MOVE TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-1);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("JUMPIFNEQ $ODD%d TF@tmp%d TF@tmp%d\n", round_counter, tmp_count-1, tmp_count-2);
	//parne
	printf("FLOAT2R2OINT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-3);
	//neparne
	printf("LABEL $ODD%d\n", round_counter);
	printf("FLOAT2R2EINT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-3);
	tmp_count++;
	round_counter++;

	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("MOVE TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-1);
	printf("FLOAT2INT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("DIV TF@tmp%d TF@tmp%d float@2.0\n", tmp_count, tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("MOVE TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-1);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("JUMPIFNEQ $ODD%d TF@tmp%d TF@tmp%d\n", round_counter, tmp_count-1, tmp_count-2);
	//parne
	printf("FLOAT2R2OINT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-3);
	//neparne
	printf("LABEL $ODD%d\n", round_counter);
	printf("FLOAT2R2EINT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-3);
	tmp_count++;
	round_counter++;

	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count-5, tmp_count-5);

	printf("PUSHS TF@tmp%d\n", tmp_count-1);
	printf("PUSHS TF@tmp%d\n", tmp_count-5);

}

void gen_retype_double() {
	static int counter = 0;

	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("MOVE TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-1);
	printf("FLOAT2INT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("DIV TF@tmp%d TF@tmp%d float@2.0\n", tmp_count, tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("MOVE TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-1);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("JUMPIFNEQ $ODD_RES%d TF@tmp%d TF@tmp%d\n", counter, tmp_count-1, tmp_count-2);
	//parne
	printf("FLOAT2R2OINT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-3);
	//neparne
	printf("LABEL $ODD_RES%d\n", counter);
	printf("FLOAT2R2EINT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count-3);
	printf("PUSHS TF@tmp%d\n", tmp_count);
	tmp_count++;	
	counter++;
}


void gen_datastack_operation(TStack_itmtype type) {
	if(type == S_PLUS) {
		printf("ADDS\n");
	}
	else if(type == S_MINUS) {
		printf("SUBS\n");
	}
	else if(type == S_MULTI) {
		printf("MULS\n");
	}
	else if(type == S_DIV) {
		printf("DIVS\n");
	}
	else if(type == S_INT_DIV) {
		gen_datastack_roundboth();
		printf("DIVS\n");
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		printf("POPS TF@tmp%d\n", tmp_count);
		printf("FLOAT2INT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
		tmp_count++;
		printf("PUSHS TF@tmp%d\n", tmp_count-1);
	}
	else if(type == S_EQUAL) {
		printf("EQS\n");
	}
	else if(type == S_MORE) {
		printf("GTS\n");
	}
	else if(type == S_LESS) {
		printf("LTS\n");
	}
	else if(type == S_NOT_EQUAL) {
		printf("EQS\n");
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		printf("POPS TF@tmp%d\n", tmp_count);
		tmp_count++;
		printf("NOT TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1);
		printf("PUSHS TF@tmp%d\n", tmp_count-1);
	}
	else {
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		printf("POPS TF@tmp%d\n", tmp_count);
		tmp_count++;
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		printf("POPS TF@tmp%d\n", tmp_count);
		tmp_count++;
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		tmp_count++;
		if(type == S_LESS_EQUAL) {
			printf("GT TF@tmp%d TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-2, tmp_count-3);
			printf("NOT TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-1);
		}
		else if(type == S_MORE_EQUAL){
			printf("LT TF@tmp%d TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-2, tmp_count-3);
			printf("NOT TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-1);
		}
		printf("PUSHS TF@tmp%d\n", tmp_count-1);
	}
}

void gen_datastack_retypefirst() {
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	printf("PUSHS TF@tmp%d\n", tmp_count);
	tmp_count++;
}

void gen_datastack_retypesecond() {
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	tmp_count++;
}

void gen_datastack_retypeboth() {
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	printf("INT2FLOAT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
	tmp_count++;
}

void gen_variable_operation(TStack_itmtype type) {
	if(type == S_PLUS) {
		printf("ADD TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_MINUS) {
		printf("SUB TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_MULTI) {
		printf("MUL TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_DIV) {
		printf("DIV TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_INT_DIV) {
		printf("PUSHS TF@tmp%d\n", tmp_count-1);
		printf("PUSHS TF@tmp%d\n", tmp_count-2);
		gen_datastack_roundboth();
		printf("DIVS\n");
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		printf("POPS TF@tmp%d\n", tmp_count);
		printf("FLOAT2INT TF@tmp%d TF@tmp%d\n", tmp_count, tmp_count);
		tmp_count++;
		printf("PUSHS TF@tmp%d\n", tmp_count-1);
		return;
	}
	else if(type == S_EQUAL) {
		printf("EQ TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_MORE) {
		printf("GT TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_LESS) {
		printf("LT TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	}
	else if(type == S_NOT_EQUAL) {
		printf("EQ TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
		printf("NOT TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1);
	}
	else {
		printf("DEFVAR TF@tmp%d\n", tmp_count);
		tmp_count++;
		if(type == S_LESS_EQUAL) {
			printf("GT TF@tmp%d TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-2, tmp_count-3);
			printf("NOT TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-1);
		}
		else if(type == S_MORE_EQUAL){
			printf("LT TF@tmp%d TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-2, tmp_count-3);
			printf("NOT TF@tmp%d TF@tmp%d\n",tmp_count-1, tmp_count-1);
		}
	}
	printf("PUSHS TF@tmp%d\n", tmp_count-1);
}

void gen_datastack_concat() {
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("CONCAT TF@tmp%d TF@tmp%d TF@tmp%d\n", tmp_count-1, tmp_count-1, tmp_count-2);
	printf("PUSHS TF@tmp%d\n", tmp_count-1);
}

void gen_function(global_item *function) {
	printf("LABEL $%s\n", function->name);
	printf("CREATEFRAME\nPUSHFRAME\n");
	
	variable_item *params= function->params;
	while(params != NULL) {
		printf("DEFVAR LF@%s\n",params->name);
		printf("POPS LF@%s\n", params->name);
		params = params->next;
	}
}

void gen_function_call(global_item *function) {

	printf("CALL $%s\n", function->name);
	
}

void gen_function_return() {
	printf("POPFRAME\n");
	printf("RETURN\n");
}

void gen_scope(){
	printf("\n");
	printf("# ----------MAIN-------------\n");
	printf("\n");
	printf("LABEL $scope\n");
	printf("CREATEFRAME\nPUSHFRAME\n");
}

void gen_assignment(char *var_name){
	printf("POPS LF@%s\n", var_name);
}

void gen_var(variable_item *item){
	printf("DEFVAR LF@%s\n", item->name);
	if(item->variable_type == INTEGER) {
		printf("MOVE LF@%s int@0\n", item->name);
	}
	else if(item->variable_type == DOUBLE) {
		printf("MOVE LF@%s float@0.0\n", item->name);
	}
	else if(item->variable_type == STRING) {
		printf("MOVE LF@%s string@\n", item->name);
	}
}
// podmienka
void gen_if() {
	static int if_count = 0;
	Gstack_push(if_stack, if_count);
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("JUMPIFNEQ $false_label%d TF@tmp%d bool@TRUE\n", if_count, tmp_count-1);
	if_count++;
}

void gen_jumpif() {
	printf("JUMP $end_if%d\n", Gstack_top(if_stack));
	printf("LABEL $false_label%d\n", Gstack_top(if_stack));
}

void gen_endif() {
	printf("LABEL $end_if%d\n", Gstack_top(if_stack));
	Gstack_pop(if_stack);
}

void gen_while_label() {
	static int while_count = 0;
	Gstack_push(while_stack, while_count);
	printf("LABEL $while%d\n", while_count);
	while_count++;
}

void gen_while_if() {
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("JUMPIFNEQ $while_finish%d TF@tmp%d bool@TRUE\n", Gstack_top(while_stack), tmp_count-1);
}

void gen_while_jump() {

	printf("JUMP $while%d\n", Gstack_top(while_stack));
	printf("LABEL $while_finish%d\n", Gstack_top(while_stack));
	Gstack_pop(while_stack);
}


void gen_print() {
	printf("DEFVAR TF@tmp%d\n", tmp_count);
	printf("POPS TF@tmp%d\n", tmp_count);
	tmp_count++;
	printf("WRITE TF@tmp%d\n", tmp_count-1);
}

void gen_input(char *name, sem_types type) {
	printf("WRITE string@?\\032\n");
	if(type == INTEGER) {
		printf("READ LF@%s int\n", name);
	}
	else if(type == DOUBLE) {
		printf("READ LF@%s float\n", name);
	}
	else if(type == STRING) {
		printf("READ LF@%s string\n", name);
	}
}

void gen_no_return(sem_types type) {
	if(type == STRING) {
		printf("PUSHS string@\n");
	}
	else if(type == INTEGER) {
		printf("PUSHS int@0\n");
	}
	else if(type == DOUBLE) {
		printf("PUSHS float@0.0\n");
	}
	printf("POPFRAME\n");
	printf("RETURN\n");
}

void gen_createframe() {
	printf("CREATEFRAME\n");
}