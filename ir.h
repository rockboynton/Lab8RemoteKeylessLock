/*
 * ir.h 
 * 
 * API for interfacing with the MSOE dev board infared decoder
 */

//Include Guards 
#ifndef IR_H_
#define IR_H_

#define TIM2_BASE (volatile uint32_t*) 0

#define NVIC_ISER_0 (volatile uint32_t*) 0xE000E100
#define CLK_SPEED 16000000L

/**
 * Initialize the Speaker GPIO port and peripheral clocks.
 */
void ir_init();

/**
 * Get code from IR signal
 */
uint32_t ir_get_code();


#endif