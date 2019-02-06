/*
 * ir.h 
 * 
 * API for interfacing with the MSOE dev board infared decoder
 */

//Include Guards 
#ifndef IR_H_
#define IR_H_

/**
 * Initialize the Speaker GPIO port and peripheral clocks.
 */
void ir_init();

/**
 * Get code from IR signal
 */
uint32_t ir_get_code();


#endif