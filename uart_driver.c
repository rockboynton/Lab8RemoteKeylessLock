/*
 * uart_driver.c
 *
 *  Created on: Nov 8, 2016
 *      Author: barnekow
 */
#include "uart_driver.h"
#include "gpio.h"
#include <inttypes.h>
#include <stdio.h>
#include "ringbuffer.h"

static volatile USART_T* USART = 0x40004400;
static volatile RingBuffer sendBuffer = {0, 0, 0};
static volatile RingBuffer recieveBuffer = {0, 0, 0};


void init_usart2(uint32_t baud, uint32_t sysclk){
	// Enable clocks for GPIOA and USART2
	*(RCC_AHB1ENR) |= (1<<GPIOAEN);
	*(RCC_APB1ENR) |= (1<<USART2EN);

	// Function 7 of PORTA pins is USART
	*(GPIOA_AFRL) &= (0xFFFF00FF); // Clear the bits associated with PA3 and PA2
	*(GPIOA_AFRL) |= (0b01110111<<8);  // Choose function 7 for both PA3 and PA2
	*(GPIOA_MODER) &= (0xFFFFFF0F);  // Clear mode bits for PA3 and PA2
	*(GPIOA_MODER) |= (0b1010<<4);  // Both PA3 and PA2 in alt function mode

	*NVIC_ISER_1 |= (1<<6); // Allows interrupt to go through

	// Set up USART2
	//USART2_init();  //8n1 no flow control
	// over8 = 0..oversample by 16
	// M = 0..1 start bit, data size is 8, 1 stop bit
	// PCE= 0..Parity check not enabled
	// using interrupts
	*(USART_CR1) = (1<<UE)|(1<<TE)|(1<<RE)|(1<<RXNEIE); // Enable UART, Tx and Rx
	*(USART_CR2) = 0;  // This is the default, but do it anyway
	*(USART_CR3) = 0;  // This is the default, but do it anyway
	*(USART_BRR) = sysclk/baud;

	/* I'm not sure if this is needed for standard IO*/
	 //setvbuf(stderr, NULL, _IONBF, 0);
	 //setvbuf(stdin, NULL, _IONBF, 0);
	 setvbuf(stdout, NULL, _IONBF, 0);
}


// Some code for Recieving (get from recieve buffer)
char usart2_getch() {
	return get(&recieveBuffer);
}

// Some code for Transmitting (put in send buffer)
void usart2_putch(char c){
	put(&sendBuffer, c);
	USART->CR1 |= (1<<TXEIE);
}

void USART2_IRQHandler(void){
	// ISR to handle RXNE interrupts
	if ((USART->SR & (1<<RXNE)) == (1<<RXNE)) {
		char c = USART->DR; // Put data into transmit buffer, sends data
		put(&sendBuffer, c);
		if (c == '\r'){  // If character is CR
			usart2_putch('\n');  // send it
			c = '\n';   // Return LF. fgets is terminated by LF
		}
		put(&recieveBuffer, c);
	}

	// ISR to handle TXE interrupts
	if ((USART->SR & (1<<TXE)) == (1<<TXE)) {
		if (hasElement(&sendBuffer)) { 
			USART->DR = get(&sendBuffer); // get data from transmit buffer, sends data
		} else {
			USART->CR1 &= ~(1<<TXEIE); // Disable TXE interrupts
		}
	}	
}
