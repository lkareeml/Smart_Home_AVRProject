/****************************************/
/*          Author: Kareem Atef         */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: Dimmer              */
/****************************************/

#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../MCAL/Interrupt/Interrupt.h"
#include "../../MCAL/Timers/Timers.h"
#include "../../AVR32_Reg_Private.h"
#include "../../MCAL/UART/UART.h"
#include "Dimmer.h"
#include <avr/interrupt.h>


extern uint8 G_Dimmer_Value;

ISR(TIMER0_OVF_vect){
	static uint8 Dimmercounter = 0;
	if( Dimmercounter < G_Dimmer_Value){
		DIO_Set_Pin_Output(Dimmer_Port,Dimmer_Pin,High);
	}else if(Dimmercounter >= G_Dimmer_Value){
		DIO_Set_Pin_Output(Dimmer_Port,Dimmer_Pin,Low);
	}
	Dimmercounter++;
	if (Dimmercounter >=253){
		Dimmercounter =0;
	}
}

void Dimmer_Init(){
	DIO_Set_Pin_Direction(Dimmer_Port,Dimmer_Pin,Out);
	Timer0_Init();
	Timer0_Enable_Overflow_Interrupt();	
}

void Dimmer_Increase(){
	if(G_Dimmer_Value < 250){
		G_Dimmer_Value+=50;
	}
}
void Dimmer_Decrease(){
	if(G_Dimmer_Value >= 50){
		G_Dimmer_Value-=50;
	}
}


void Dimmer_FeedBack(uint8 New_Feed,Requester Type){
	//1 increase, 2 decrease, 3 off, 4 on
	switch(New_Feed){
		case 1: 
			Dimmer_Increase();
			if(Type == UART) UART_Dimmer_Show_Up();
			break;
		case 2: 
			Dimmer_Decrease();
			if(Type == UART) UART_Dimmer_Show_Down();
			break;
		case 3: 
			if(G_Dimmer_Value == 0){}
			else if(G_Dimmer_Value > 0){
				G_Dimmer_Value = 0;
			}
			break;
		case 4: 
			if(G_Dimmer_Value == 0){
				Dimmer_Increase();
			}
			else if(G_Dimmer_Value > 0){}
			break;
	}
}
