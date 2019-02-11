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
#include "ringbuffer.h"

static volatile GPIO* GPIOB = 0x40020400;
static volatile TIMER* TIM2 = TIM2_BASE;

static uint32_t last_timestamp; // Stored as clock ticks of 50us each due to prescaler
static uint32_t elapsed_time;
static uint8_t num_bits_read;
static RingBuffer key_buffer = {0, 0, 0};
static uint32_t current_key;

/* Define the states and events.*/
static enum states { 
    WAITING_FOR_START, WAITING_FOR_ENDSTART, WAITING_FOR_BIT, MAX_STATES 
    } current_state;
static enum events { IRQ, MAX_EVENTS } new_event;

/* Provide the fuction prototypes for each action procedure.*/
static void action_WAITING_FOR_START_IRQ (void);
static void action_WAITING_FOR_ENDSTART_IRQ (void);
static void action_WAITING_FOR_BIT_IRQ (void);

/* Define the state/event lookup table. */

static void (*const state_table [MAX_STATES][MAX_EVENTS]) (void) = {

    action_WAITING_FOR_START_IRQ, action_WAITING_FOR_ENDSTART_IRQ , 
    action_WAITING_FOR_BIT_IRQ  

};

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

    /* Configure TIM2 (pg.538) */
    // ?????
    // CC4 channel is configured as input, IC4 is mapped on TI4
    TIM2->CCMR2 = (TIM2->CCMR2 & ~(0b11 << 8)) | (0b01 << 8); 
    // Falling edge only selected
    TIM2->CCER |= (1 << 13);
    // Interrupt request on input capture, channel 4
    TIM2->DIER |= (1 << 4);
    // Prescaler set as 800 for 50us clock tick
    TIM2->PSC = 800; 
    // Auto-reload set as max value of 32 bit number
    TIM2->ARR = 0xFFFFFFFF;
    // Enable capture for channel 4
    TIM2->CCER |= (1 << 12);
    // Enable counter
    TIM2->CR1 |= 1;
    // ????

    // Enable Interrupts
    *(NVIC_ISER_0) |= (1 << 30);

    current_state = WAITING_FOR_START;
    num_bits_read = 0;
    current_key = 0;
}

uint32_t ir_get_code() {
    uint32_t code = 0;
    // Wait for key press
    while (!hasElement(&key_buffer)) {
        // wait
    }
    code = get(&key_buffer);
    return code; 
}

static uint32_t ir_get_code_no_block() {
    //TODO
    return 0;
}

// Called whenever there is a falling edge event
TIM2_IRQHandler(void) {
    // pg 367 ref manual
    state_table [current_state][IRQ] (); /* call the action procedure */
}

static void action_WAITING_FOR_START_IRQ (void) {
    last_timestamp = TIM2->CCR4;
    current_state = WAITING_FOR_ENDSTART;
}

static void action_WAITING_FOR_ENDSTART_IRQ (void) { 
    elapsed_time = TIM2->CCR4 - last_timestamp;
    if (240 < elapsed_time && elapsed_time < 300) {
        last_timestamp = TIM2->CCR4;
        current_state = WAITING_FOR_BIT;
    } else {
        // ? error
        current_state = WAITING_FOR_START;
    }
}

static void action_WAITING_FOR_BIT_IRQ (void) {
    elapsed_time = TIM2->CCR4 - last_timestamp;
    if (20 < elapsed_time && elapsed_time < 30) {
        last_timestamp = TIM2->CCR4;
        current_key &= ~(1 << num_bits_read); // bit was a 0
        num_bits_read++;
    } else if (30 < elapsed_time && elapsed_time < 60) {
        current_key |= (1 << num_bits_read); // bit was a 1
        num_bits_read++;
    } else {
        // ? error
        current_state = WAITING_FOR_START;
        num_bits_read = 0;
    }

    if (num_bits_read > 32) {
        put(&key_buffer, current_key);
        num_bits_read = 0;
        current_state = WAITING_FOR_START;
    }
}
