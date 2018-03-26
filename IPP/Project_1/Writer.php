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
 * \class Writer
 * \brief Creating XML document from instrucitons of IPPcode18 
 * 
 * Class uses XMLWriter to creat new XML document from code IPPcode18
 *
 * \author $Author: Jakub Crkoň
 */
class Writer 
{
	/**
 	* @typedef Writer properties
 	* @{
 	*/
 	// XMLWriter isntance
	private $writer;
	// array containing instruction and operands
	private $instruction;

	/** @} */

	/** 
	* \brief Constructor of Writer
 	* 
	* Instance of class creates new XMLWriter
	* Set memory as output , delimeter as tabulator
 	* Start document with needed version and coding
 	* Create first element with needed atributes(without closing)
 	*
 	*/
	function __construct() 
	{
        $this->writer = new XMLWriter(); 
		$this->writer->openMemory();
		$this->writer->setIndent(true);
		$this->writer->setIndentString('	');
		$this->writer->startDocument( '1.0' , 'UTF-8' );
		$this->writer->startElement('program'); 
		$this->writer->writeAttribute('language','IPPcode18');
    }
	/** 
	* \brief Print XML document from memroy to STDOUT
 	* 
	* Function ends first element and prints document to STDOUT
 	*
 	*/
   	public function output_xml()
    {
    	$this->writer->endElement();
    	$this->writer->endDocument();
    	echo $this->writer->outputMemory();
 
    }
	/** 
	* \brief Write instrucion and its operands into the XML document
 	* 
	* Function create elements of instruction in property $strings and insert needed attributes
	* using method write_atribute()
 	*
 	*/
  	public function write_instruction($op_arr, $order) 
  	{	
  		$this->instruction = $op_arr;
  		$this->writer->startElement('instruction'); 
		$this->writer->writeAttribute('order', $order);
		$this->writer->writeAttribute('opcode', $this->instruction[0]);
		for($i = 1; $i < count($this->instruction); $i++) {
			$this->writer->startElement('arg'.$i); 
			$this->write_attribute($i);
			$this->writer->endElement();

		}
		$this->writer->endElement(); 
  	}

	/** 
	* \brief Insert attribute needed for instruction
 	* 
	* Function check which attribute is needed according to instruction and index of operand,
	* then insert it into last created element using methods write_symbol, write_label, write_type
	*
 	* \param $index - postion of operand in property $string
 	*/
  	private function write_attribute($index) 
	{	
		//instructions which needs attribute
		switch($this->instruction[0]) {
			case "INT2CHAR":
			case "STRLEN":
			case "TYPE":
			case "MOVE":
			case "ADD":
			case "SUB":
			case "MUL":
			case "IDIV":
			case "LT":
			case "GT":
			case "EQ":
			case "AND":
			case "OR":
			case "NOT":
			case "STRI2INT":
			case "CONCAT":
			case "GETCHAR":
			case "SETCHAR":
			case "PUSHS":
			case "WRITE":
			case "DPRINT":
			case "DEFVAR":
			case "POPS":
				$this->write_symbol($index);
				break;
			case "LABEL":
			case "JUMP":
			case "CALL":
				$this->write_label();
				break;
			case "JUMPIFEQ":
			case "JUMPIFNEQ":
				if($index == 1)
					$this->write_label();
				else
					$this->write_symbol($index);
				break;
			case "READ":
				$this->write_type();
				break;
		}
	}
	/** 
	* \brief Insert attribute for symbol
 	* 
 	* Insert needed attribute according to operand
 	* Can insert varaible or constant
	*
 	* \param $index - postion of operand in property $string
 	*/
  	private function write_symbol($index)
  	{
  		$type = strstr($this->instruction[$index] ,'@', true);
  		if(preg_match('/^(TF|LF|GF)$/', $type)){
  			$this->writer->writeAttribute('type', 'var');
  			$this->writer->text($this->instruction[$index]); 
  		}
  		else {
  			$this->writer->writeAttribute('type', $type);
  			$value = substr($this->instruction[$index], strpos($this->instruction[$index], "@") + 1);
  			$this->writer->text($value); 
  		}
  	}
	/** 
	* \brief Insert attribute for symbol
 	* 
 	* Insert attribute LABEL
 	*/
	private function write_label()
  	{
  		$this->writer->writeAttribute('type', 'label');
  		$this->writer->text($this->instruction[1]); 
  	}
	/** 
	* \brief Insert attribute for symbol
 	* 
 	* Insert attribute TYPE
 	*/
  	  private function write_type()
  	{
  		$this->writer->writeAttribute('type', 'type');
  		$this->writer->text($this->instruction[2]); 
  	}

}

?>