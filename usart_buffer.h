/*
 * ringbuffer.h 
 * 
 * API for using a ring buffer for the purpose of storing key presses
 */

#include <inttypes.h>

#ifndef USARTt_BUFFER_H_
#define USART_BUFFER_H_

#define BUF_SIZE 15

typedef struct {
	unsigned int put;
	unsigned int get; 
    unsigned int used; // number of elements currently in the buffer 
	char buffer[BUF_SIZE]; 
} UsartBuffer;

// Adds element to buffer.  Will block if there is no space in buffer.
void usart_put(UsartBuffer* buffer, char element);

// Gets element from buffer.  Will block if buffer is empty.
char usart_get(UsartBuffer* buffer);

// Returns true (non-zero) if there is room for one element in buffer
int usart_hasSpace(UsartBuffer *);

// Return true (non-zero) if there is at least one element in buffer
int usart_hasElement(UsartBuffer *);

// Return 1 for same, 0 for mismatch
int usart_cmp(UsartBuffer* buffer1, UsartBuffer* buffer2);

#endif
