/****************************************/
/*          Author: Kareem Atef         */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: Buzzer              */
/****************************************/

#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../MCAL/Interrupt/Interrupt.h"


void BUZZER_Initialization()
{
	DIO_Set_Pin_Direction(PORTA,Pin3,Out);
}

void BUZZER_On()
{
	DIO_Set_Pin_Output(PORTA,Pin3,High);
}

void BUZZER_Off()
{
	DIO_Set_Pin_Output(PORTA,Pin3,Low);
}

void BUZZER_ALARM_TILL_RESET()
{
	BUZZER_On();
	GIE_Disable();
	while(1);
}