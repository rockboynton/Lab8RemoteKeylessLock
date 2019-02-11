/*
 * ringbuffer.c
 *
 *  Created on: Jan 28, 2019
 *      Author: Rock Boynton
 * 
 * Implements functions specified in ringbuffer.h to use a ring buffer for the purpose of storing 
 * key presses
 */

#include "ringbuffer.h";
#include <inttypes.h>

void put(RingBuffer* buffer, uint32_t element) {
    while (!hasSpace(buffer)) {
    }
    buffer->buffer[buffer->put] = element;
    // check if wrap needed
    if (buffer->put >= BUF_SIZE) {
        buffer->put = 0; // wrap back to 0
    } else {
        buffer->put++;
    }
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
    buffer->used--;
    return element;
}

int hasSpace(RingBuffer* buffer) { 
    return BUF_SIZE != buffer->used;
}

int hasElement(RingBuffer* buffer) {
    return buffer->used > 0;
}
