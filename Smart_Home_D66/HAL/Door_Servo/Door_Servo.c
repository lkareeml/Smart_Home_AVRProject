/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 29-11-2023		    */
/*          Version: 2.0				*/
/*          Module: Door_Servo          */
/****************************************/
#define F_CPU 16000000UL
#include <util/delay.h>
#include "../../STD_Types.h"
#include "../../Bit_Manipulation.h"
#include "../../MCAL/Timers/Timers.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../MCAL/Interrupt/Interrupt.h"
#include "../../AVR32_Reg_Private.h"
#include "../../MCAL/UART/UART.h"
#include "Door_Servo.h"
#include <avr/interrupt.h>

/*
Open Door 180 degree :	50HZ, duty cycle 12.3%
	HIGH  2.5  ms, LOW 17.5 ms
Closed Door 0 degree: 50HZ, duty cycle 2.3%
	HIGH 0.5  ms, LOW  19.5 ms
	
	50 HZ  ==> Wave(20ms) time 0.5 ms, from 0 to 40
	00.0 (send HIGH)				0
	00.5 (if closed send LOW)		1
	02.5 (if open Send Low)			5
	...
	19.5 
	20.0							40
*/

extern uint8 door_state;// 0 is closed, 1 is open

void Door_Feedback(uint8 New_Feed,Requester Type){
	switch(New_Feed){
		case 2: 
			door_state = 0;
			if(Type == UART) UART_Door_Show_Closed();
			break;
		case 1:
			door_state = 1;
			if(Type == UART) UART_Door_Show_Open();
			break;
	}
}

void Servo_Init(){
	DIO_Set_Pin_Direction(Servo_Port,Servo_Pin,Out);
	DIO_Set_Pin_Output(Servo_Port,Servo_Pin,Low);
	// Set the Prescaler to 8
	// Set WGM12 to enable CTC mode
	TCCR1B_Reg |= (1 << WGM12) | (1 << CS11);
	// Set OCR1A to generate an interrupt every 0.5 ms
	// Set TOP value for 0.5ms period
	// Set OCR1A to 1000
	OCR1AH_Reg = 0x03; // High byte 00000011
	OCR1AL_Reg = 0xE8; // Low byte 11101000    0000001111101000
	// Enable Timer/Counter1 Output Compare Match A interrupt
	TIMSK_Reg |= (1 << OCIE1A);
	GIE_Enable(); // Enable global interrupts
}