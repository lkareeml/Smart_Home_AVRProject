/****************************************/
/*          Author : Kareem Atef        */
/*          Date: 06-12-2023		    */
/*          Version: 2.0				*/
/*          Module: 5-LEDs              */
/****************************************/

#include "../../MCAL/Digital_Input_Output/DIO.h"
#include "../../Bit_Manipulation.h"
#include "../../AVR32_Reg_Private.h"
#include "Leds.h"

void LED_Init_All(){
	DIO_Set_Pin_Direction(PORTCx,2,Out);
	DIO_Set_Pin_Direction(PORTCx,7,Out);
	DIO_Set_Pin_Direction(PORTDx,3,Out);
	DIO_Set_Pin_Direction(PORTDx,4,Out);
	DIO_Set_Pin_Direction(PORTDx,5,Out);
	DIO_Set_Pin_Direction(PORTAx,0,Out);
}
void LED0_Init(){DIO_Set_Pin_Direction(PORTCx,2,Out);}
void LED1_Init(){DIO_Set_Pin_Direction(PORTCx,7,Out);}
void LED2_Init(){DIO_Set_Pin_Direction(PORTDx,3,Out);}
void LED3_Init(){DIO_Set_Pin_Direction(PORTDx,4,Out);}
void LED4_Init(){DIO_Set_Pin_Direction(PORTDx,5,Out);}
void Lamp_Dimmer_Init(){DIO_Set_Pin_Direction(PORTAx,0,Out);}


void LED_0_On(){DIO_Set_Pin_Output(PORTCx,2,High);}
void LED_1_On(){DIO_Set_Pin_Output(PORTCx,7,High);}
void LED_2_On(){DIO_Set_Pin_Output(PORTDx,3,High);}
void LED_3_On(){DIO_Set_Pin_Output(PORTDx,4,High);}
void LED_4_On(){DIO_Set_Pin_Output(PORTDx,5,High);}
void Lamp_Dimmer_On(){DIO_Set_Pin_Output(PORTAx,0,High);}

void LED_0_Off(){DIO_Set_Pin_Output(PORTCx,2,Low);}
void LED_1_Off(){DIO_Set_Pin_Output(PORTCx,7,Low);}
void LED_2_Off(){DIO_Set_Pin_Output(PORTDx,3,Low);}
void LED_3_Off(){DIO_Set_Pin_Output(PORTDx,4,Low);}
void LED_4_Off(){DIO_Set_Pin_Output(PORTDx,5,Low);}
void Lamp_Dimmer_Off(){DIO_Set_Pin_Output(PORTAx,0,Low);}

void LED_0_Toggle(){Toggle(PORTC_Reg,2);}
void LED_1_Toggle(){Toggle(PORTC_Reg,7);}
void LED_2_Toggle(){Toggle(PORTD_Reg,3);}
