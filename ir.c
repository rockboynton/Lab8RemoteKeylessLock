/*
 * ir.c
 *
 *  Created on: Feb 12, 2019
 *      Author: Rock Boynton
 * 
 * Implements functions specified in ir.h to use the MSOE dev board infared decoder
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "ir.h"
#include "gpio.h"

static volatile GPIO* GPIOB = 0x40020400;

// File scope helper methods
static uint32_t ir_get_code_block();

void ir_init() {
    // Enable GPIOB in RCC_AHB1ENR
	*(RCC_AHB1ENR) |= (1 << GPIOBEN);
    // Configure GPIOB pin 1 to analog mode
    GPIOB->MODER |= (0b11 << 2);

}

uint32_t ir_get_code() {
    uint32_t code = 0;
    // Wait for key press
    while (code == 0) {
        code = ir_get_code_no_block();
        delay_1ms(50);
    }
    // Wait for key release
    while (ir_get_code()) {
        delay_1ms(50);
    }
    return code; 
}

static uint32_t ir_get_code_no_block() {
    //TODO
    return 0;
}