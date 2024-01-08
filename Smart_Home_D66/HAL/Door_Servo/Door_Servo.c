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
#include <avr/interrupt.h>


static uint8 door_state=0;// 0 is closed, 1 is open

/*
Open Door lets make it to 180 degree which means:
	duty cycle 12.3% , 50HZ
	HIGH  for 2.5  ms // LOW   for 17.5 ms
	
Closed Door lets make it to 0 degree which means:
	duty cycle 2.3% , 50HZ
	HIGH  for 0.5  ms // LOW   for 19.5 ms
	
	50 HZ  == 20ms time 0.5 ms for each step
	so total counter is from 0 to 40
	00.0 (if open send High)	(if closed send HIGH)		0
	00.5						(if closed send LOW)		1
	02.5 (if open Send Low)									5
	...
	19.5 
	20.0													40
	
*/


ISR(TIMER1_COMPA_vect) // Timer1 compare match interrupt
{
	static uint8 counter = 0;
	// Start if counter = 0 by making Servo Pin high
	if(counter==0){DIO_Set_Pin_Output(PORTD,Pin7,High);}
	else if(counter == 1){
		if(door_state == 0){
			DIO_Set_Pin_Output(PORTD,Pin7,Low);
		}
	}
	else if(counter == 5){
		if(door_state == 1){
			DIO_Set_Pin_Output(PORTD,Pin7,Low);
		}
	}
	if(counter<40){counter++;}
	else {counter = 0;}
}


void Servo_Init(){
	DIO_Set_Pin_Direction(PORTD,Pin7,Out);
	DIO_Set_Pin_Output(PORTD,Pin7,Low);
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



/*
void Servo_Init(){
    DIO_Set_Pin_Direction(PORTD,Pin7,Out);
    DIO_Set_Pin_Output(PORTD,Pin7,Low);
}

void Servo_0_Degree_Delay(){
	//duty cycle 2.3% , 50HZ
    DIO_Set_Pin_Output(PORTD,Pin7,High);
    _delay_us(500);
    DIO_Set_Pin_Output(PORTD,Pin7,Low);
    _delay_ms(19.5);
}

void Servo_90_Degree_Delay(){
	//duty cycle 7.3% , 50HZ
	DIO_Set_Pin_Output(PORTD,Pin7,High);
	_delay_us(1500);
	DIO_Set_Pin_Output(PORTD,Pin7,Low);
	_delay_ms(18.5);
}
void Servo_180_Degree_Delay(){
	//duty cycle 12.3% , 50HZ
	DIO_Set_Pin_Output(PORTD,Pin7,High);
	_delay_us(2500);
	DIO_Set_Pin_Output(PORTD,Pin7,Low);
	_delay_ms(17.5);
}
*/
/*
  Set timer interrupt after 1 ms;
• Signal pin High at the 1st ISR;
• At the next ISR Set Signal pin to LOW
• Count another 18 ISR;
• Repeat the cycle again.
*/
