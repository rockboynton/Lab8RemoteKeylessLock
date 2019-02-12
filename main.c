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
#include "ir.h"

#define F_CPU 16000000UL

// 0 = main program
// 1 = Speaker Tests
// 2 = IR tests
// 3 = USART tests
#define DEBUG 3

// Defines
#define LOCK_BUTTON 0 // TODO fill this in when value is found
#define UNLOCK_BUTTON 1 // TODO fill this in when value is found

// Constants
static const Tone UNLOCK_SOUND[] = {
	// {NOTE, DURATION}
	{A6, S}, 
	{C0, S}, // C0 is inaudible, acts as a delay to seperate notes
    {A6,S},
};

static const Tone LOCK_SOUND[] = {
	// {NOTE, DURATION}
	{A6, S}, 
};


// Initialize variables to be used 


// File Scope helper methods
static void play_sound(Tone sound[], int length);

/* Define the states and events.*/
static enum states { LOCKED, UNLOCKED, MAX_STATES } current_state;
static enum events { UNLOCK, LOCK, MAX_EVENTS } new_event;

/* Provide the fuction prototypes for each action procedure.*/
static void action_LOCKED_UNLOCK (void);
static void action_LOCKED_LOCK (void);
static void action_UNLOCKED_UNCLOCK (void);
static void action_UNLOCKED_LOCK (void);
static enum events get_new_event (void);

/* Define the state/event lookup table. */

void (*const state_table [MAX_STATES][MAX_EVENTS]) (void) = {

    { action_LOCKED_UNLOCK, action_LOCKED_LOCK }, /* procedures for state 1 */
    { action_UNLOCKED_UNCLOCK, action_UNLOCKED_LOCK } /* procedures for state 2 */

};

static char buffer[50];


// main
int main() {
	
	// Initialize hardware
	init_usart2(19200,F_CPU);
	led_init();
	lcd_init();
	key_init();
	temp_init();
	speaker_init();
	ir_init();

	// --------------------Speaker Tests----------------------------
	while(DEBUG == 1) { // Working
		play_sound(UNLOCK_SOUND, (sizeof(UNLOCK_SOUND) / sizeof(UNLOCK_SOUND[0])));
		delay_1ms(3000);
		play_sound(LOCK_SOUND, (sizeof(LOCK_SOUND) / sizeof(LOCK_SOUND[0])));
		delay_1ms(3000);
	}
	//-------------------------------------------------------------

	// --------------------IR Tests----------------------------
	while(DEBUG == 2) { // Not yet working 
		lcd_print_num(ir_get_code());
		delay_1ms(2000);
		lcd_clear();
		delay_1ms(2000);
	}
	//-------------------------------------------------------------

		// --------------------IR Tests----------------------------
	while(DEBUG == 3) { // Not yet working 
		printf("Enter your name: ");
		scanf("%s", buffer);
		printf("Your name is %s\n", buffer);
	}
	//-------------------------------------------------------------


	// Main program 
	// Never return
	while (1) {
		new_event = get_new_event (); /* get the next event to process */

    	if (((new_event >= 0) && (new_event < MAX_EVENTS))
		&& ((current_state >= 0) && (current_state < MAX_STATES))) {

        	state_table [current_state][new_event] (); /* call the action procedure */

    	} else {
        	/* invalid event/state - ignore */
    	}
	}
	// Never returns
	return 0;
}

static void play_sound(Tone sound[], int length) {
	for (int i = 0; i < length; i++) {
		play_tone(&(sound[i]));
	}
}

// --------------------------------------------------------------------------------
/* In an action procedure, you do whatever processing is required for the
particular event in the particular state. */

static void action_LOCKED_UNLOCK (void) {
	play_sound(UNLOCK_SOUND, (sizeof(UNLOCK_SOUND) / sizeof(UNLOCK_SOUND[0])));
	lcd_print_string("UNLOCKED");
	current_state = UNLOCKED;
}

static void action_LOCKED_LOCK (void) {
	action_UNLOCKED_LOCK();
}

static void action_UNLOCKED_UNCLOCK (void) {
	action_LOCKED_UNLOCK();
}

static void action_UNLOCKED_LOCK (void) {
	play_sound(LOCK_SOUND, (sizeof(LOCK_SOUND) / sizeof(LOCK_SOUND[0])));
	lcd_print_string("LOCKED");
	current_state = LOCKED;
}
// --------------------------------------------------------------------------------

/* Return the next event to process. */

static enum events get_new_event (void) {
	uint32_t code = ir_get_code();
	enum events new_event;
	switch (code) {
		case LOCK_BUTTON : 
			new_event = LOCK;
			break;
		case UNLOCK_BUTTON : 
			new_event = UNLOCK;
			break;
		default : // INVALID BUTTON
			new_event = -1;	
	}
    return new_event;
}

