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
#include "../../MCAL/UART/UART.h"
#include <avr/interrupt.h>

#define Dimmer_Pin		3
#define Dimmer_Port		PORTDx


extern uint8 G_Dimmer_Value;

ISR(TIMER0_OVF_vect){
	static uint8 Dimmercounter = 0;
	if( Dimmercounter < G_Dimmer_Value){
		DIO_Set_Pin_Output(Dimmer_Port,Dimmer_Pin,High);
	}else if(Dimmercounter >= G_Dimmer_Value){
		DIO_Set_Pin_Output(Dimmer_Port,Dimmer_Pin,Low);
	}
	Dimmercounter++;
	if (Dimmercounter >=254){
		Dimmercounter =0;
	}
}

void Dimmer_Init()
{
	DIO_Set_Pin_Direction(Dimmer_Port,Dimmer_Pin,Out);
	Timer0_Init();
	Timer0_Enable_Overflow_Interrupt();	
}



void Dimmer_Feedback(uint8 New_Feed){
	//1 increase
	//2 decrease
	//3 turn off
	//4 turn on
	switch(New_Feed){
		case 1: if(G_Dimmer_Value == 250){
					UART_Send_String_Polling_8("Already MAX!");
				}
				else if(G_Dimmer_Value < 250){
					G_Dimmer_Value+=50;
					UART_Send_String_Polling_8("Dimmer Increased!");
				}
			break;
		case 2: 
			if(G_Dimmer_Value ==0){
				UART_Send_String_Polling_8("Already MIN!");
				}
			else if(G_Dimmer_Value > 0){
				G_Dimmer_Value -=50;
				UART_Send_String_Polling_8("Dimmer Decreased!");
				}
			break;
		case 3: 
			if(G_Dimmer_Value == 0){UART_Send_String_Polling_8("Already OFF!");}
			else if(G_Dimmer_Value > 0){
				G_Dimmer_Value = 0;
				UART_Send_String_Polling_8("Turn Off Success!");}
			break;
		case 4: 
			if(G_Dimmer_Value == 0){
				G_Dimmer_Value+=50;
				UART_Send_String_Polling_8("Turn On Success!");}
			else if(G_Dimmer_Value > 0){UART_Send_String_Polling_8("Already ON!");}
			break;
	}
}
	
void Dimmer_Increase(){
	if(G_Dimmer_Value < 250){
		G_Dimmer_Value++;
	}
}
void Dimmer_Decrease(){
	if(G_Dimmer_Value >= 50){
		G_Dimmer_Value--;
	}
}
void Dimmer_On(){
	G_Dimmer_Value = 50;	
}
void Dimmer_Off(){
	G_Dimmer_Value = 0;
}




