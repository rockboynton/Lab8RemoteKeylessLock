/*
 * main.c
 *
 * Program for an IR remote locking system
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
#define DEBUG 0

// ------------------------------------TEAC RC-505 Button codes-------------------------------------
#define BUTTON_LOCK (uint32_t) 0
#define BUTTON_UNLOCK (uint32_t) 0
#define BUTTON_1 (uint32_t) 3977404806
#define BUTTON_2 (uint32_t) 3960693126
#define BUTTON_3 (uint32_t) 0
#define BUTTON_4 (uint32_t) 0
#define BUTTON_5 (uint32_t) 0
#define BUTTON_6 (uint32_t) 0
#define BUTTON_7 (uint32_t) 0
#define BUTTON_8 (uint32_t) 0
// -------------------------------------------------------------------------------------------------


// ---------------------------------------Program Tones---------------------------------------------
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
// -------------------------------------------------------------------------------------------------

// File Scope helper methods
static void play_sound(Tone sound[], int length);

/*********************************** Define the states and events.*********************************/
static enum states { LOCKED, UNLOCKED, MAX_STATES } current_state;
static enum events { UNLOCK, LOCK, MAX_EVENTS } new_event;

/********************* Provide the fuction prototypes for each action procedure.*******************/
static void action_LOCKED_UNLOCK (void);
static void action_LOCKED_LOCK (void);
static void action_UNLOCKED_UNCLOCK (void);
static void action_UNLOCKED_LOCK (void);
static enum events get_new_event (void);

/******************************** Define the state/event lookup table. ****************************/

void (*const state_table [MAX_STATES][MAX_EVENTS]) (void) = {

    { action_LOCKED_UNLOCK, action_LOCKED_LOCK }, /* procedures for state 1 */
    { action_UNLOCKED_UNCLOCK, action_UNLOCKED_LOCK } /* procedures for state 2 */

};

// Variables to be used
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
	while(DEBUG == 2) { // Working
		lcd_print_num(ir_get_code());
	}
	//-------------------------------------------------------------

		// --------------------IR Tests----------------------------
	while(DEBUG == 3) { // Not tested
		printf("Enter your name: ");
		scanf("%s", buffer);
		printf("Your name is %s\n", buffer);
	}
	//-------------------------------------------------------------


	// Main program 
	// Never return
	while (1) {
		/* get the next event to process */
		new_event = get_new_event (); 

    	if (((new_event >= 0) && (new_event < MAX_EVENTS)) 
		&& ((current_state >= 0) && (current_state < MAX_STATES))) {
			/* call the action procedure */
        	state_table [current_state][new_event] (); 
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

/***************************************************************************************************
 In an action procedure, you do whatever processing is required for the particular event in the particular state. 
 **************************************************************************************************/
static void action_LOCKED_UNLOCK (void) {
	play_sound(UNLOCK_SOUND, (sizeof(UNLOCK_SOUND) / sizeof(UNLOCK_SOUND[0])));
	lcd_clear();
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
	lcd_clear();
	lcd_print_string("LOCKED");
	current_state = LOCKED;
}

/***************************** Return the next event to process. **********************************/
static enum events get_new_event (void) {
	// uint32_t code = ir_get_code();
	uint32_t code = ir_get_code();
	enum events new_event;
	switch (code) {
		case BUTTON_1 :
			new_event = LOCK;
			break;
		case BUTTON_2 :
			new_event = UNLOCK;
			break;
		default : // INVALID BUTTON
			new_event = -1;	
	}
    return new_event;
}
