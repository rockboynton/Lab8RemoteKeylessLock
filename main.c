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
#define DEBUG 0

// User input buffer
#define BUFFER_SIZE 30

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

	// Main program 
	// Never return
	while (1) {
		printf("What is your name?\n");
        scanf("%s", buffer);
        printf("Your name is %s.\n", buffer);
	}
	// Never returns
	return 0;
}


