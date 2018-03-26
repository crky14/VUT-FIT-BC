<?php
/**
 * Name: Project IPP 2018 - 'parse.php'
 * Class: IPP
 * Year: 2017/2018
 * University: VUTBR FIT
 * Login: xcrkon00
 * Names: Jakub Crkoň
 */

/**
 * \class Error_types
 *
 * \brief Class contains error codes
 * 
 * Class is contains constants representing errors produced by 
 * class 'Scanner.php' and error exit codes.
 *
 * \author $Author: Jakub Crkoň
 */
abstract class Error_types
{
	// Header missing
	const header_error = 0;
	// Wrong instruction
	const instruction_error = 1;
	// Wrong operand for instruction
	const operands_error = 2;
	// Wrong variable name syntax
	const variable_error = 3;
	// Wrong symbol syntax
	const symbol_error = 4;
	// Wrong type syntax
	const type_error = 5;
	// Wrong label name syntax 
	const label_error = 6;

	// SYNTAX ERROR
	const SYNTAX_ERROR = 21;
}

?>
