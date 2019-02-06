/*
 * ringbuffer.h 
 * 
 * API for using a ring buffer for the purpose of buffering 
 * asynchronous serial I/O.
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#define BUF_SIZE 15

typedef struct {
	unsigned int put;
	unsigned int get; 
    unsigned int used; // number of elements currently in the buffer 
	char buffer[BUF_SIZE]; 
} RingBuffer;

// Adds element to buffer.  Will block if there is no space in buffer.
void put(RingBuffer* buffer, char element);

// Gets element from buffer.  Will block if buffer is empty.
char get(RingBuffer* buffer);

// Returns true (non-zero) if there is room for one element in buffer
int hasSpace(RingBuffer *);

// Return true (non-zero) if there is at least one element in buffer
int hasElement(RingBuffer *);

#endif