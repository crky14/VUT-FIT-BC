/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xcrkon00, xblask04, xbarte14, xkalus05
 * Names: Jakub Crkoň, Barbora Blašková, Bronislav Bárteček, Katarína Kalusová
 */
#include <stdlib.h>
#include <stdio.h>
#include "token_handler.h"
#include "lexical_analysis.h"
#include "parser.h"


int main(){


	if(init_tables() == 99) {
		exit_program(99);
	}
	if(Gstack_init() == 99) {
		exit_program(99);
	}
	init_tokenlist();
	gen_init();

	int ret = syntax_check();
	
	exit_program(ret);

	return 0;
}
