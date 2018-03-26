<?php
/**
 * Name: Project IPP 2018 - 'parse.php'
 * Class: IPP
 * Year: 2017/2018
 * University: VUTBR FIT
 * Login: xcrkon00
 * Names: Jakub Crkoň
 */
include 'Error_types.php';

/**
 * \class Scanner
 *
 * \brief Class checking syntax of code IPPcode18
 * 
 * Class reads IPPcode18 from STDIN. 
 * Checks syntax and lexical correctness.
 * In case of error exits with needed return code and prints 
 * information about error to STDERR 
 *
 * \author $Author: Jakub Crkoň
 */
class Scanner 
{
	/**
 	* @typedef Scanner properties
 	* @{
 	*/
	
	// array of opperands for instruction last loaded instruction
	private $op_arr;
	// count of all rows 
	private $row_count = 0;
	// count of rows with instruction
	private $order_count = 0;
	// count of row with comment
	private $comment_count = 0;

	/** @} */

	/** 
	* \brief Scan instruction and checks it correctness
 	* 
 	* Function loads line from STDIN and calss another protected methods of class for
 	* and checking lexical correctness and syntax.
 	* Methods skips comments rows and if needed cuts them off the instruction.
 	*
 	* \return Returns true if instruction is ok , fail in case it is not
 	*/
	public function scan_instruction()
	{
		//loads instruction into row , skipping empty rows and full row comments
		$row;
		do {
			if(!($row = fgets(STDIN)))
				return false;
			if(ctype_space($row)) {
				continue;
			}
			$this->row_count++; //increasing $row_count
			if(strpos($row, '#') === false)
				break;
			$this->comment_count++; //incresing $comments_count if needed
		}while(empty($row = strstr($row, '#', true)));

		$this->order_count++;
		$this->op_arr = preg_split('/[\s]+/', $row, -1, PREG_SPLIT_NO_EMPTY);
		//checking syntax and lexical correctness
		$this->check_syntax();
		return true;
		
	}
	/** 
	* \brief Scan input for header 'IPPcode18'
 	* 
 	* Function reads line from STDIN and check 
 	* header correctness
 	* If header is missing exits using method error_handler.
 	* 
 	*/
	public function scan_header()
	{	
		//Reads rows skipping comments
		$row;
		do {
			$row = fgets(STDIN);
			if(ctype_space($row)) {
				continue;
			}
			$this->row_count++;
			if(strpos($row, '#') === false)
				break;
			$this->comment_count++;
		}while(empty($row = strstr($row, '#', true)));

		//header is case insensitive so its converted to lower
		$row = trim(strtolower($row));
		if(strcmp($row,".ippcode18")) {
			$this->error_handler(Error_types::header_error);
		}
	}

	/** 
	* \brief Check if op_arr contains needed count of operands for instruction
	*
 	* Exiting using method error_handler() if count of opperand is not correct
 	* 
 	* \param $needed - count of needed operands
 	*/
	private function check_op_count($needed) {
		if(count($this->op_arr) - 1 != $needed) {
			$this->error_handler(Error_types::operands_error);				
		}
	}
	/** 
	* \brief Check lexical correctness of variable name using regular expression comparison
	*
 	* Exiting using method error_handler() if varaible name is not correct
 	* 
 	* \param $index - index in op_arr on which is operand located
 	*/
	private function check_variable($index) {
		if(!preg_match('/^(TF|LF|GF)[@][a-zA-Z-_\$&%\*][0-9a-zA-Z-_\$&%\*]*$/', $this->op_arr[$index])){
			$this->error_handler(Error_types::variable_error);
		}
	}
	/** 
	* \brief Check correctness of label name using regular expression comparison
	*
 	* Exiting using method error_handler() if label name is not correct
  	* 
 	* \param $index - index in op_arr on which is operand located
 	*/
	private function check_label($index) {
		if(!preg_match('/^[a-zA-Z-_\$&%\*][0-9a-zA-Z-_\$&%\*]*$/', $this->op_arr[$index])){
			$this->error_handler(Error_types::label_error);
		}
	}
	/** 
	* \brief Check lexical correctness of symbol using regular expression comparison
	*
 	* Exiting using method error_handler() if symbol is not correct
  	* 
 	* \param $index - index in op_arr on which is operand located
 	*/
	private function check_symbol($index) {
		if(!preg_match('/^((TF|LF|GF)@[a-zA-Z-_\$&%\*][0-9a-zA-Z-_\$&%\*]*|((int@)(-?[0-9]*)|(bool@)(true|false)|(string@)((\\\\[0-9]{3})|[^\n \t\\\\#])*))$/', $this->op_arr[$index])){
			$this->error_handler(Error_types::symbol_error);
		}		
	}
	/** 
	* \brief Check lexical correctness of type using regular expression comparison
	*
 	* Exiting using method error_handler() if type is not correct
  	* 
 	* \param $index - index in op_arr on which is operand located
 	*/
	private function check_type($index) {
		if(!preg_match('/^(int|bool|string)$/', $this->op_arr[$index])){
			$this->error_handler(Error_types::type_error);
		}
	}
	/** 
	* \brief Cheks syntax correctness
	*
	* Method is checks syntax fo instruction and calls lexical analysis functions
	* for each operand of instruction.
 	* Exiting using method error_handler() if syntax error is found
 	*/
	private function check_syntax() 
	{	
		// changing instruction name to upper
		$this->op_arr[0] = strtoupper($this->op_arr[0]);

		switch($this->op_arr[0]) {
			case "INT2CHAR":
			case "STRLEN":
			case "TYPE":
			case "MOVE":
			case "NOT":
				$this->check_op_count(2);
				$this->check_variable(1);
				$this->check_symbol(2);
				break;
			case "CREATEFRAME":
			case "POPFRAME":
			case "PUSHFRAME":
			case "BREAK":
			case "RETURN":
				$this->check_op_count(0);
				break;
			case "DEFVAR":
			case "POPS":
				$this->check_op_count(1);
				$this->check_variable(1);
				break;
			case "PUSHS":
			case "WRITE":
			case "DPRINT":
				$this->check_op_count(1);
				$this->check_symbol(1);
				break;
			case "ADD":
			case "SUB":
			case "MUL":
			case "IDIV":
			case "LT":
			case "GT":
			case "EQ":
			case "AND":
			case "OR":
			case "STRI2INT":
			case "CONCAT":
			case "GETCHAR":
			case "SETCHAR":
				$this->check_op_count(3);
				$this->check_variable(1);
				$this->check_symbol(2);
				$this->check_symbol(3);
				break;
			case "LABEL":
			case "JUMP":
			case "CALL":
				$this->check_op_count(1);
				$this->check_label(1);
				break;
			case "JUMPIFEQ":
			case "JUMPIFNEQ":
				$this->check_op_count(3);
				$this->check_label(1);
				$this->check_symbol(2);
				$this->check_symbol(3);
				break;
			case "READ":
				$this->check_op_count(2);
				$this->check_type(2);
				break;
			default:
				$error_handler(Error_types::instruction_error);
		}
	}
	/** 
	* \brief Returns count of comments
	*
	* \return integer number
 	*/
	public function return_comnt_count() {
		return $this->comment_count;
	}
	/** 
	* \brief Returns array containing instrucion and operands
	*
	* \return Array of strings
 	*/
	public function return_operands() {
		return $this->op_arr;
	}
	/** 
	* \brief Returns instruciton order
	*
	* \return integer number
 	*/
	public function return_order_count() {
		return $this->order_count;
	}
	/** 
	* \brief Function prints Error message to STDERR and exit with error code
	*
 	*/
	private function error_handler($Error_instance) {
		fwrite(STDERR, 'Error has occured on line '.$this->row_count);
		if($Error_instance == 0)
			fwrite(STDERR, ' - Header is missing'. PHP_EOL);
		else if($Error_instance == 1)
			fwrite(STDERR, ' - Wrong instruction'. PHP_EOL);
		else if ($Error_instance == 2) 
			fwrite(STDERR, ' - Wrong count of operands for instrucion'. PHP_EOL);
		else if($Error_instance == 3)
			fwrite(STDERR, ' - Wrong variable name'. PHP_EOL);
		else if($Error_instance == 4)
			fwrite(STDERR, ' - Wrong constant value'. PHP_EOL);
		else if($Error_instance == 5)
			fwrite(STDERR, ' - Wrong type'. PHP_EOL);
		else if($Error_instance == 6)
			fwrite(STDERR, ' - Wrong label name'. PHP_EOL);
		exit(Error_types::SYNTAX_ERROR);
	}
}

?>
