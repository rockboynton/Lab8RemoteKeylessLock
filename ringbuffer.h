/*
 * ringbuffer.h 
 * 
 * API for using a ring buffer for the purpose of storing key presses
 */

#include <inttypes.h>

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#define BUF_SIZE 10

typedef struct {
	unsigned int put;
	unsigned int get; 
    unsigned int used; // number of elements currently in the buffer 
	uint32_t buffer[BUF_SIZE]; 
} RingBuffer;

// Adds element to buffer.  Will block if there is no space in buffer.
void put(RingBuffer* buffer, uint32_t element);

// Gets element from buffer.  Will block if buffer is empty.
uint32_t get(RingBuffer* buffer);

// Returns true (non-zero) if there is room for one element in buffer
int hasSpace(RingBuffer *);

// Return true (non-zero) if there is at least one element in buffer
int hasElement(RingBuffer *);

#endif
