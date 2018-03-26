/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xbarte14
 * Names: Bronislav Bárteček
 */

#include "buffer.h"
#include <limits.h>

TBuffer * buffer_init(){
	TBuffer * buffer;
	buffer = (TBuffer *) malloc(sizeof(TBuffer));
	if (buffer == NULL){
		exit_program(99);
	}

	buffer->data = malloc(sizeof(char)*buffer_size);
	if(buffer->data == NULL){
		exit_program(99);
	}

	for (int i = 0; i < buffer_size; i++) {
		buffer->data[i] = '\0';
	}
	buffer->size = 0;
	buffer->capacity = buffer_size;

	return buffer;
}

void buffer_add(char c,TBuffer *buffer){
	if(buffer->size >= (buffer->capacity - 1)){	//-1 bcs last \0 need to be there
		buffer->data = realloc(buffer->data,(sizeof(char)*(buffer->capacity + buffer_size)));
		if (buffer->data == NULL){
			exit_program(99);
		}
		for (int i = buffer->capacity; i < (buffer->capacity + buffer_size); i++) {
			buffer->data[i] = '\0';
		}
		buffer->capacity += buffer_size;
	}
	buffer->data[buffer->size]=c;
	buffer->size++;
}

void buffer_free(TBuffer *buffer){
	free(buffer->data);
	free(buffer);
}

long int * buffer_to_int(TBuffer *buffer){
	long int * number;
	number = malloc(sizeof(long int));
	if (number == NULL) {
		exit_program(99);
	}
	*number = strtol(buffer->data, NULL,10);
	if(*number == LONG_MAX || *number == LONG_MIN) exit(1);
	return number;
}

double * buffer_to_double(TBuffer *buffer){
	double * number;
	number = malloc(sizeof(double));
	if (number == NULL) {
		exit_program(99);
	}
	*number = atof(buffer->data);
	return number;
}

char * buffer_to_char(TBuffer *buffer){
	char * chardata;
	chardata = malloc(sizeof(char)*(buffer->size+1));
	if(chardata == NULL){
		exit_program(99);
	}
	for (int i = 0; i < (buffer->size+1); i++) {
		chardata[i] = '\0';
	}
	for (int i = 0; i < (buffer->size+1); i++) {
		chardata[i] = buffer->data[i];

	}
	return  chardata;
}

