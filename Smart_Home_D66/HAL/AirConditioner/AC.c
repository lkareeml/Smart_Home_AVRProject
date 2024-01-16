/****************************************/
/*          Author: Kareem              */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: AC                  */
/****************************************/

#include "../../STD_Types.h"
#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "AC.h"
#include "../../MCAL/UART/UART.h"
#include "../../Global_Var.h"

extern uint8 AC_State;

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

void AC_Feedback(uint8 New_Feed){
	//1 Means Turn Automatic AC
	//2 Means Turn Off AC Manual
	//3 Means Turn On AC Manual
	switch(New_Feed){
		case 1:
			if(AC_State == 2){
					UART_Send_String_Polling_8("AC is Already Auto!");
			}else{
				AC_State = 2;
				UART_Send_String_Polling_8("AC Auto Success!");
			}
			break;
		case 2:
				if(AC_State == 0){
					UART_Send_String_Polling_8("AC is Already Closed!");
				}else{
					AC_Off();
					UART_Send_String_Polling_8("AC Turned Off Success!");
				}
			break;
		case 3:
			if(AC_State == 1){
				UART_Send_String_Polling_8("AC Already Turned ON!");
			}else{
				AC_On();
				UART_Send_String_Polling_8("AC Turned On Success!");
			}
			break;
		default:UART_Show_Invalid();break;
	}
}