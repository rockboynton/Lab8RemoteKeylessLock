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
static volatile TIMER* TIM2 = TIM2_BASE;

// File scope helper methods
static uint32_t ir_get_code_no_block();

void ir_init() {
    // Enable GPIOB in RCC_AHB1ENR
	*(RCC_AHB1ENR) |= (1 << GPIOBEN);

     // Enable TIM2 in RCC_APB1ENR
	*(RCC_APB1ENR) |= (1 << TIM2_EN);

    // Configure GPIOB pin 2 to alternate function mode (TIM2)
    GPIOB->MODER &= ~(0b11 << 4); // Clear mode bits for pin 2
    GPIOB->MODER |= (ALTERNATE_FUNCTION << 4); // Set pin 2 to alternate function mode
    GPIOB->AFRL &= ~(0xF << 8); // Clear alternate function bits for pin 2
    GPIOB->AFRL |= (0x2 << 8); // Set alternate function to AF1 (TIM2_CH4)

    // Enable Interrupts
    *NVIC_ISER_0 |= (1 << 30);

    TIM2->EGR |= 1; // Propogate new values from shadow registers

    // ? TODO more stuff to enable reading from IR sensor

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

TIM4_IRQHandler(void) {
    // pg 367 ref manual
}
