/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xblask04
 * Names: Barbora Blašková
 */
#include <stdio.h>
#include <stdlib.h> 
#include "exit.h"
#include "token_handler.h"

void exit_program(int return_value){
	garbage_collector();
	printf("#Compilation terminated with return code of %d\n", return_value);
	exit(return_value);
}
