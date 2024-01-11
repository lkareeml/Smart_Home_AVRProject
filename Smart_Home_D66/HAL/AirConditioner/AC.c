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



uint8 AC_State_Auto = 0;
uint8 AC_State = 0;

void AC_Initialization()
{
	DIO_Set_Pin_Direction(AC_Port,AC_Pin,Out);
}
void AC_On(){
	DIO_Set_Pin_Output(AC_Port,AC_Pin,High);
}
void AC_Off(){
	DIO_Set_Pin_Output(AC_Port,AC_Pin,Low);
}

void AC_Feedback(uint8 New_Feed){
	//1 Means Turn Automatic AC
	//2 Means Turn Off AC Manual
	//3 Means Turn On AC Manual
	switch(New_Feed){
		case 1:
			switch(AC_State_Auto){
				case 0 :
				AC_State_Auto = 1;
				UART_Send_String_Polling_8("AC Auto Success!");
				break;
				case 1:
				UART_Send_String_Polling_8("AC is Already Auto!");
				break;
			}
			break;
		case 2:
			switch(AC_State){
				case 0 :
					UART_Send_String_Polling_8("AC is Already Closed!");
					break;
				case 1:
					AC_Off();
					AC_State = 0;
					AC_State_Auto = 0;
					UART_Send_String_Polling_8("AC Turned Off Success!");
					break;
			}
			break;
		case 3:
			switch(AC_State){
				case 0 :
					AC_On();
					AC_State = 1;
					AC_State_Auto = 0;
					UART_Send_String_Polling_8("AC Turned On Success!");
					break;
				case 1:
					UART_Send_String_Polling_8("AC Already Turned ON!");
					break;
			}
			break;
		default:UART_Show_Invalid();break;
	}
}