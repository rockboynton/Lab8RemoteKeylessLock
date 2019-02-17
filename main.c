/*
 * main.c
 *
 * Program for an IR remote locking system
 * 				
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
#define BUTTON_LOCK 	(uint32_t) 4177944966 // shuffle key
#define BUTTON_UNLOCK 	(uint32_t) 4111098246 // time
#define BUTTON_1 		(uint32_t) 3977404806
#define BUTTON_2 		(uint32_t) 3960693126
#define BUTTON_3 		(uint32_t) 3994116486
#define BUTTON_4 		(uint32_t) 4010828166
#define BUTTON_5 		(uint32_t) 4027539846
#define BUTTON_6 		(uint32_t) 3877134726
#define BUTTON_7 		(uint32_t) 3893846406
#define BUTTON_8 		(uint32_t) 3943981446
#define BUTTON_9 		(uint32_t) 3927269766
#define BUTTON_0 		(uint32_t) 3910558086
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

/*********************************** States and Events.********************************************/
static enum states { LOCKED, UNLOCKED, MAX_STATES } current_state;
static enum events { UNLOCK, LOCK, ENTER_DIGIT, MAX_EVENTS } new_event;

/********************* Function prototypes for each action procedure.******************************/
static void action_LOCKED_UNLOCK (void);
static void action_LOCKED_LOCK (void);
static void action_LOCKED_ENTER_DIGIT (void);
static void action_UNLOCKED_UNCLOCK (void);
static void action_UNLOCKED_LOCK (void);
static void action_UNLOCKED_ENTER_DIGIT (void);
static enum events get_new_event (void);

/******************************** State/Event lookup table. ***************************************/
void (*const state_table [MAX_STATES][MAX_EVENTS]) (void) = {

    { action_LOCKED_UNLOCK, action_LOCKED_LOCK, action_LOCKED_ENTER_DIGIT },/* Locked State */
    { action_UNLOCKED_UNCLOCK, action_UNLOCKED_LOCK, action_UNLOCKED_ENTER_DIGIT } /* Unlocked State */

};

const char* STATE_NAMES[] = {"LOCKED", "UNLOCKED"};

// Constants to be used 
#define MAX_SIZE 6
#define MIN_SIZE 4

// Variables to be used
static char buffer[50];
static uint32_t code_buffer[MAX_SIZE];
static uint32_t entry_buffer[MAX_SIZE];
static uint32_t current_code;
static uint8_t current_index = 0;

static void display_code(uint32_t code);

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
		uint32_t key_code = ir_get_code();
	}
	//-------------------------------------------------------------

		// --------------------IR Tests----------------------------
	while(DEBUG == 3) { // Not tested
		printf("Enter your name: ");
		scanf("%s", buffer);
		printf("Your name is %s\n", buffer);
	}
	//-------------------------------------------------------------

	// Initial state is unlocked
	current_state = UNLOCKED;
	lcd_print_string("UNLOCKED");
	// Main program...Never return
	while (1) {
		/* get the next event to process */
		new_event = get_new_event (); 

    	if (((new_event >= 0) && (new_event < MAX_EVENTS)) 
		&& ((current_state >= 0) && (current_state < MAX_STATES))) {
			/* call the action procedure */
        	state_table [current_state][new_event] (); 
    	} else {
        	/* invalid event/state */
			// do nothing
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

/************************************* Action Procedures ******************************************/
static void action_LOCKED_UNLOCK (void) {
	if (memcmp(code_buffer, entry_buffer, sizeof(code_buffer)) == 0) { // code match
		play_sound(UNLOCK_SOUND, (sizeof(UNLOCK_SOUND) / sizeof(UNLOCK_SOUND[0])));
		lcd_clear();
		lcd_print_string("UNLOCKED");
		current_state = UNLOCKED;
	} else {
		lcd_clear();
		lcd_print_string("Invalid code");
		delay_1ms(1000);
		lcd_clear();
		lcd_print_string("LOCKED");
	}
	memset(entry_buffer, 0, sizeof(entry_buffer)); // clear entry buffer
	current_index = 0;
}

static void action_LOCKED_LOCK (void) {
	current_index = 0;
	memset(entry_buffer, 0, sizeof(entry_buffer)); // clear entry buffer
	play_sound(LOCK_SOUND, (sizeof(LOCK_SOUND) / sizeof(LOCK_SOUND[0])));
	lcd_clear();
	lcd_print_string("LOCKED");
	current_state = LOCKED;
}

static void action_LOCKED_ENTER_DIGIT (void) {
	if (current_index == MAX_SIZE) {
		memset(entry_buffer, 0, sizeof(entry_buffer)); // clear entry buffer
		lcd_clear();
		lcd_print_string("Incorrect code");
		delay_1ms(1000);
		lcd_clear();
		lcd_print_string("LOCKED");
		current_index = 0;
	} else {
		if (current_index == 0) {
			lcd_clear();
		}
		entry_buffer[current_index] = current_code;
		current_index++;
		display_code(current_code);
	}
}

static void action_UNLOCKED_UNCLOCK (void) {
	play_sound(UNLOCK_SOUND, (sizeof(UNLOCK_SOUND) / sizeof(UNLOCK_SOUND[0])));
	lcd_clear();
	lcd_print_string("UNLOCKED");
	current_state = UNLOCKED;
	current_index = 0;
}

static void action_UNLOCKED_LOCK (void) {
	if (MIN_SIZE <= current_index && current_index <= MAX_SIZE) {
		action_LOCKED_LOCK();
	} else {
		lcd_clear();
		lcd_print_string("Invalid code");
		delay_1ms(1000);
		lcd_clear();
		lcd_print_string("UNLOCKED");
		current_index = 0;
	}
}

static void action_UNLOCKED_ENTER_DIGIT (void) {
	if (current_index == MAX_SIZE) {
		memset(code_buffer, 0, sizeof(code_buffer)); // clear code buffer
		lcd_clear();
		lcd_print_string("Invalid code");
		delay_1ms(1000);
		lcd_clear();
		lcd_print_string("UNLOCKED");
		current_index = 0;
	} else {
		if (current_index == 0) {
			lcd_clear();
		}
		code_buffer[current_index] = current_code;
		current_index++;
		display_code(current_code);		
	}
}

/***************************** Return the next event to process. **********************************/
static enum events get_new_event (void) {
	current_code = ir_get_code();
	switch (current_code) {
		case BUTTON_LOCK :
			new_event = LOCK;
			break;
		case BUTTON_UNLOCK :
			new_event = UNLOCK;
			break;
		case BUTTON_0 :
		case BUTTON_1 :		
		case BUTTON_2 :
		case BUTTON_3 :
		case BUTTON_4 :
		case BUTTON_5 :
		case BUTTON_6 :
		case BUTTON_7 :
		case BUTTON_8 :
		case BUTTON_9 :
			new_event = ENTER_DIGIT;
			break;
		default : // INVALID BUTTON
			new_event = -1;	
	}
    return new_event;
}

static void display_code(uint32_t code) {
	uint32_t num;
	switch (code) {
		case BUTTON_0 :
			num = 0;
			break;
		case BUTTON_1 :	
			num = 1;
			break;	
		case BUTTON_2 :
			num = 2;
			break;
		case BUTTON_3 :
			num = 3;
			break;
		case BUTTON_4 :
			num = 4;
			break;
		case BUTTON_5 :
			num = 5;
			break;
		case BUTTON_6 :
			num = 6;
			break;
		case BUTTON_7 :
			num = 7;
			break;
		case BUTTON_8 :
			num = 8;
			break;
		case BUTTON_9 :
			num = 9;
			break;
		default: 
			num = 9999;
			break;
	}
	lcd_print_num(num);
}
