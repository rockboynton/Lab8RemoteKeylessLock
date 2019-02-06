/*
 * ringbuffer.c
 *
 *  Created on: Jan 28, 2019
 *      Author: Rock Boynton
 * 
 * Implements functions specified in ringbuffer.h to use a ring buffer for the purpose of buffering 
 * asynchronous serial I/O.
 */

#include "ringbuffer.h";

void put(RingBuffer* buffer, char element) {
    while (!hasSpace(buffer)) {
    }
    buffer->buffer[buffer->put] = element;
    // check if wrap needed
    if (buffer->put >= BUF_SIZE) {
        buffer->put = 0; // wrap back to 0
    } else {
        buffer->put++;
    }
    // modulo_inc(buffer->put, BUF_SIZE);
    buffer->used++;
}

char get(RingBuffer* buffer) {
    while (!hasElement(buffer)) {
    }
    char element = buffer->buffer[buffer->get];
    if (buffer->get >= BUF_SIZE) {
        buffer->get = 0;
    } else {
        buffer->get++;
    }
    // modulo_inc(buffer->get, BUF_SIZE);
    buffer->used--;
    return element;
}

int hasSpace(RingBuffer* buffer) { 
    return BUF_SIZE != buffer->used;
}

int hasElement(RingBuffer* buffer) {
    return buffer->used > 0;
}
