/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xbarte14
 * Names: Bronislav Bárteček
 */
/**
 * @buffer
 * Buffer documentation
 */

#ifndef IFJ_BBJK_BUFFER_H
#define IFJ_BBJK_BUFFER_H

#include <stdlib.h>
#include <string.h>
#include "exit.h"
#define buffer_size 16

/**
 * @typedef buffer structures
 * \@{
 */
typedef struct _TBuffer{
	int size;
	int capacity;
	char * data;
}TBuffer;
/** @} */

/**
 * @typedef buffer functions
 * \@{
 */
/**
 * Allocate memory for buffer and fill it with values
 * capacity is buffer_size
 * @return pointer to struct TBuffer
 */
TBuffer * buffer_init();

/**
 * Add char in the end of buffer
 * in case that buffer is full it reallocate memory
 * There is alwais /0 in the end
 * @param c			char, which will be added
 * @param buffer	pointer to struct TBuffer, structure for change
 */
void buffer_add(char c,TBuffer *buffer);

/**
 * Free the memory from struct TBuffer
 * @param buffer 	pointer to struct TBuffer, which will be deleted
 * @pre buffer != NULL
 */
void buffer_free(TBuffer *buffer);

/**
 * Takes buffer and makes from char integer value
 * Value will be stored in "number" which is pointer to integer
 * It does NOT free buffer, for that use buffer_free function after this
 * @param buffer	pointer to buffer, from which will be data taken
 * @return	ponter to integer
 * @pre buffer != NULL
 */
long int * buffer_to_int(TBuffer *buffer);

/**
 * Takes buffer and makes from char double value
 * Value will be stored in "number" which is pointer to double
 * It does NOT free buffer, for that use buffer_free function after this
 * @param buffer	pointer to buffer, from which will be data taken
 * @return	ponter to double
 * @pre buffer != NULL
 */
double * buffer_to_double(TBuffer *buffer);

/**
 * Takes buffer and makes from char char value
 * (just pointer of char, no free bytes, no structure, size is exactly that we need)
 *  * It does NOT free buffer, for that use buffer_free function after this
 * @param buffer	pointer to buffer, from which will be data taken
 * @return pointer to char
 * @pre buffer != NULL
 */
char * buffer_to_char(TBuffer *buffer);
/** @} */

#endif //IFJ_BBJK_BUFFER_H
