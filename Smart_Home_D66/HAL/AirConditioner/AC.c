/****************************************/
/*          Author: Kareem              */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: AC                  */
/****************************************/

#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../MCAL/UART/UART.h"
#include "../../Global_Var.h"
#include "../../HAL/LCD_16x2/LCD.h"
#include "AC.h"


extern uint8 AC_State;
extern uint8 AC_Auto;

void AC_Initialization()
{
	DIO_Set_Pin_Direction(AC_Port,AC_Pin,Out);
}
void AC_On(){
	DIO_Set_Pin_Output(AC_Port,AC_Pin,High);
	AC_State = 1;
}
void AC_Off(){
	DIO_Set_Pin_Output(AC_Port,AC_Pin,Low);
	AC_State = 0;
}

void AC_Feedback(uint8 New_Feed,Requester Type){
	//1 Means Turn Automatic AC
	//2 Means Turn Off AC Manual
	//3 Means Turn On AC Manual
	switch(New_Feed){
		case 1:
			AC_Auto = 1;
			if(Type == UART) UART_AC_Show_Auto();
			break;
		case 2:
			AC_Auto = 0;
			AC_Off();
			if(Type == UART){ 
				UART_AC_Show_Manual();
				UART_AC_Show_Off();
			}
			break;
		case 3:
			AC_Auto = 0;
			AC_On();
			if(Type == UART){
				UART_AC_Show_Manual();
				UART_AC_Show_On();
			}
			break;
		default:break;
	}
}