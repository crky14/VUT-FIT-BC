<?php
/**
 * Name: Project IPP 2018 - 'parse.php'
 * Class: IPP
 * Year: 2017/2018
 * University: VUTBR FIT
 * Login: xcrkon00
 * Names: Jakub Crkoň
 */
include 'Scanner.php';
include 'Writer.php';

// Help message for script 'parse.php'
const help_message = 'Script is scanning source code IPPcode18 from STDIN and producing XML code to  STDOUT'.PHP_EOL.'Usage :'.PHP_EOL.'Script can be executed from command line using file \'php5.6 parse.php\''.PHP_EOL.'Script can be executed with following command line arguments :'.PHP_EOL.'--help : Print help message to STDIN'.PHP_EOL.'--stats=\'path\' : Print statistic about source code to file, must be combined with at least one of following tWo arguments'.PHP_EOL.'	--loc : Print count of source code lines with instruction'.PHP_EOL.'	--comments : Print count of lines with comment'.PHP_EOL;

/** 
 * \brief Function print error message to STDERR and exit with error code
 */
function command_error() {
		fwrite(STDERR, 'Wrong usage of command line options'. PHP_EOL);
		exit(10);
}

// Array of command lines arguments
$longopts = array(
	"help",
	"stats:",
	"loc",
	"comments"
);
//getting command line arguments using getopts()
$options = getopt("", $longopts);

//check for invalid argument on command line
if(count($options) != $argc - 1) {
	command_error();
}
//check if help is needed
if(array_key_exists("help", $options)) {
	//help cant be combined with another command line arguments, exiting 10
	if(count($options) != 1 or $argc != 2){
		command_error();
	}
	echo $help = help_message;
	exit(0);
}

//$stats is flag for checking if command line argument stats was inserted
$stats = false;
if(array_key_exists("stats", $options)) {
	$stats = true;
	//--stats has to be combined with at least one argument --loc or --comments
	if((!array_key_exists("comments", $options) and !array_key_exists("loc", $options)) or $options["stats"] === "false") {
		command_error();
	}
}
//creating instance of Scanner and Writer
$scanner = new Scanner();
$writer = new Writer();

//checks for needed header
$scanner->scan_header();
//Using scanner to get instruction and writer to created needed XML
while($scanner->scan_instruction()) 
	$writer->write_instruction($scanner->return_operands(), $scanner->return_order_count());

$writer->output_xml();

//trying to print statistic to file if option was selected
if($stats == true) {
	//trying create new file
	try{
		$file = fopen($options["stats"], "w");
	}
	//if acces is denied printing error message to STDERR and exiting with error code
	catch(Exception $e) {
		fwrite(STDERR, 'Error has occured - '.$e->getMessage(). PHP_EOL);
		exit(99);
	}
	//print selected statistics to file
	foreach ($options as $key => $value) {
		if(!strcmp($key, 'loc')) {
			fwrite($file, $scanner->return_order_count().PHP_EOL);
		}
		if(!strcmp($key, 'comments')) {
			fwrite($file, $scanner->return_comnt_count().PHP_EOL);
		}
	}
}
exit(0);

?>
