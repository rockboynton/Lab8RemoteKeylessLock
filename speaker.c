/*
 * speaker.c
 *
 *  Created on: Jan 19, 2019
 *      Author: Rock Boynton
 * 
 * Implements functions specified in speaker.h to use the MSOE dev board piezo speaker
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "speaker.h"
#include "delay.h"
#include "gpio.h"

static volatile GPIO* GPIOB = 0x40020400;
static volatile TIMER* TIM3 = TIM3_BASE;

void speaker_init() {
    // Enable GPIOB in RCC_AHB1ENR
	*(RCC_AHB1ENR) |= (1 << GPIOBEN);
    // Enable TIM3 in RCC_APB1ENR
	*(RCC_APB1ENR) |= (1 << TIM3_EN);
    // Configure GPIOB pin 4 for alternate function
    GPIOB->MODER &= ~(0x00000300); // Clear mode bits for pin 4
    GPIOB->MODER |= (ALTERNATE_FUNCTION << 8); // Set pin 4 to alternate function mode
    GPIOB->AFRL &= ~(0x00010000); // Clear alternate function bits for pin 4
    GPIOB->AFRL |= (0x2 << 16);
    TIM3->EGR |= 1; // Propogate new values from shadow registers
    // Configure clock source â€“ default (reset) is to use processor clock
}

void play_tone(Tone* tone) {
	// Set half-period count in TIM3_ARR and TIM3_CCR1
    uint32_t ticks = (uint32_t) (CLK_SPEED *  (1 / (*tone).note)) / 2.0;
    // uint32_t ticks = (uint16_t) (CLK_SPEED/2) / (*tone).note)) / 2.0;
    TIM3->ARR = ticks;
    TIM3->CCR1 = ticks;

    // Set output mode to â€œtoggle on matchâ€� in TIM3_CCMR1
    TIM3->CCMR1 = (0b011 << 4); // OC1M=011 - toggle on match

    // Enable output in TIM3_CCER
    TIM3->CCER = 1; // CC1E = 1

    // Enable counter in TIM3_CR1 to begin playing tone
    TIM3->CR1 = 1; // CEN = 1
    
    delay_1ms((*tone).duration);

    // Disable counter in TIM3_CR1 to end playing tone
    TIM3->CR1 = 0; // CEN = 1

    // TIM3->EGR |= 1; // Propogate new values from shadow registers
}
