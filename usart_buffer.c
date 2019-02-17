/*
 * ringbuffer.c
 *
 *  Created on: Jan 28, 2019
 *      Author: Rock Boynton
 * 
 * Implements functions specified in ringbuffer.h to use a ring buffer for the purpose of storing 
 * key presses
 */

#include "usart_buffer.h";
#include <inttypes.h>

void usart_put(UsartBuffer* buffer, char element) {
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

char usart_get(UsartBuffer* buffer) {
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

int usart_hasSpace(UsartBuffer* buffer) { 
    return BUF_SIZE != buffer->used;
}

int usart_hasElement(UsartBuffer* buffer) {
    return buffer->used > 0;
}

int usart_cmp(UsartBuffer* buffer1, UsartBuffer* buffer2) {
    return memcmp(buffer1, buffer2, sizeof(buffer1));
}
