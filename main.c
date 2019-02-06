/*
 * main.c
 *
 * Tests the functionality of UART with interrupts
 * 				
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "uart_driver.h"
#include "led.h"
#include "lcd.h"
#include "keypad.h"
#include "delay.h"
#include "temp_sensor.h"
#include "speaker.h"

#define F_CPU 16000000UL

// 0 = main program
// 1 = Speaker Tests
#define DEBUG 1

// User input buffer
#define BUFFER_SIZE 30

// Constants
static const Tone UNLOCK[] = {
	// {NOTE, DURATION}
	{A6, S}, 
	{C0, S}, // C0 is inaudible, acts as a delay to seperate notes
    {A6,S},
}

static const Tone LOCK[] = {
	// {NOTE, DURATION}
	{A6, S}, 
}

// Initialize variables to be used 
static char buffer[BUFFER_SIZE];



// main
int main() {
	
	// Initialize hardware
	init_usart2(19200,F_CPU);
	led_init();
	lcd_init();
	key_init();
	temp_init();
	speaker_init();

	// --------------------Speaker Tests----------------------------
	while(DEBUG == 1) {
		play_sound(UNLOCK, (sizeof(UNLOCK) / sizeof(UNLOCK[0])));
		delay_1ms(3000);
		play_sound(LOCK, (sizeof(LOCK) / sizeof(LOCK[0])));
		delay_1ms(3000);
	}


	// Main program 
	// Never return
	while (1) {
		// TODO 
	}
	// Never returns
	return 0;
}

static void play_sound(Tone sound[], int length) {
	for (int i = 0; i < length; i++) {
		play_tone(&(sound[i]));
	}
}

