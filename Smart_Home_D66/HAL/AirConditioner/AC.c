/****************************************/
/*          Author: Kareem              */
/*          Date: 06-12-2023            */
/*          Version: 1.0                */
/*          Module: AC                  */
/****************************************/


#include "../../MCAL/Digital_Input_Output/DIO.h"

void AC_Initialization()
{
	DIO_Set_Pin_Direction(PORTAx,Pin2,Out);
}
void AC_On(){
	DIO_Set_Pin_Output(PORTAx,Pin2,High);
}
void AC_Off(){
	DIO_Set_Pin_Output(PORTAx,Pin2,Low);
}