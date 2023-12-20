/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 29-11-2023		    */
/*          Version: 2.0				*/
/*          Module: Door_Servo          */
/****************************************/

#define F_CPU 16000000UL
#include <util/delay.h>
#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../MCAL/Timers/Timers.h"

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

/*
  Set timer interrupt after 1 ms;
• Signal pin High at the 1st ISR;
• At the next ISR Set Signal pin to LOW
• Count another 18 ISR;
• Repeat the cycle again.
*/
