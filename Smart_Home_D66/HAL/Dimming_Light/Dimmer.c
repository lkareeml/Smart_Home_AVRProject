/*
 * Dimmer.c
 *
 * Created: 12/20/2023 4:24:33 PM
 *
 */ 


#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../MCAL/Interrupt/Interrupt.h"
#include "../../MCAL/Timers/Timers.h"
#include "../../AVR32_Reg_Private.h"
void activate_bulb()
{
	Timer0_Init();//intilize the timer 
	//mode of the pwm has been selected in timer config file
	DIO_Set_Pin_Direction(PORTB,3,Out);
}
void light_bulb(int status)
{
	if(status==1)
	{
		Timer0_Set_OCR0_Reg(50);
	}
	else if(status==2)
	{
		Timer0_Set_OCR0_Reg(100);
	}
	else if(status==3)
	{
		Timer0_Set_OCR0_Reg(150);
	}
	else if(status==4)
	{
		Timer0_Set_OCR0_Reg(200);
	}
	else if(status==5)
	{
		Timer0_Set_OCR0_Reg(250);
	}
}
