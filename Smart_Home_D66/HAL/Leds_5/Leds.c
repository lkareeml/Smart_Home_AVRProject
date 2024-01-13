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

#define Led0_Pin	2
#define Led0_Port	PORTCx
#define Led1_Pin	7
#define Led1_Port	PORTCx
#define Led2_Pin	3
#define Led2_Port	PORTDx
#define Led3_Pin	5
#define Led3_Port	PORTDx
#define Led4_Pin	4
#define Led4_Port	PORTDx
#define Dimmer_Pin		0
#define Dimmer_Port		PORTAx

void LED_Init_All(){
	DIO_Set_Pin_Direction(Led0_Port,Led0_Pin,Out);
	DIO_Set_Pin_Direction(Led1_Port,Led1_Pin,Out);
	DIO_Set_Pin_Direction(Led2_Port,Led2_Pin,Out);
	DIO_Set_Pin_Direction(Led3_Port,Led3_Pin,Out);
	DIO_Set_Pin_Direction(Led4_Port,Led4_Pin,Out);
	DIO_Set_Pin_Direction(Dimmer_Port,Dimmer_Pin,Out);
}

void LED0_Init(){DIO_Set_Pin_Direction(Led0_Port,Led0_Pin,Out);}
void LED1_Init(){DIO_Set_Pin_Direction(Led1_Port,Led1_Pin,Out);}
void LED2_Init(){DIO_Set_Pin_Direction(Led2_Port,Led2_Pin,Out);}
void LED3_Init(){DIO_Set_Pin_Direction(Led3_Port,Led3_Pin,Out);}
void LED4_Init(){DIO_Set_Pin_Direction(Led4_Port,Led4_Pin,Out);}


void LED_0_On(){DIO_Set_Pin_Output(Led0_Port,Led0_Pin,High);}
void LED_1_On(){DIO_Set_Pin_Output(Led1_Port,Led1_Pin,High);}
void LED_2_On(){DIO_Set_Pin_Output(Led2_Port,Led2_Pin,High);}
void LED_3_On(){DIO_Set_Pin_Output(Led3_Port,Led3_Pin,High);}
void LED_4_On(){DIO_Set_Pin_Output(Led4_Port,Led4_Pin,High);}

void LED_0_Off(){DIO_Set_Pin_Output(Led0_Port,Led0_Pin,Low);}
void LED_1_Off(){DIO_Set_Pin_Output(Led1_Port,Led1_Pin,Low);}
void LED_2_Off(){DIO_Set_Pin_Output(Led2_Port,Led2_Pin,Low);}
void LED_3_Off(){DIO_Set_Pin_Output(Led3_Port,Led3_Pin,Low);}
void LED_4_Off(){DIO_Set_Pin_Output(Led4_Port,Led4_Pin,Low);}

void LED_0_Toggle(){DIO_Set_Pin_Output(Led0_Port,Led0_Pin,Toggle);}
void LED_1_Toggle(){DIO_Set_Pin_Output(Led1_Port,Led1_Pin,Toggle);}
void LED_2_Toggle(){DIO_Set_Pin_Output(Led2_Port,Led2_Pin,Toggle);}
void LED_3_Toggle(){DIO_Set_Pin_Output(Led3_Port,Led3_Pin,Toggle);}
void LED_4_Toggle(){DIO_Set_Pin_Output(Led4_Port,Led4_Pin,Toggle);}
void Dimmer_Toggle(){DIO_Set_Pin_Output(Dimmer_Port,Dimmer_Pin,Low);}

void LED_Feedback(uint8 g_choice_2){
	uint8 x = 0;
	switch(g_choice_2){
		case 1: // Toggle Led 0
		LED_0_Toggle(); x = 0;break;
		case 2: // Toggle Led 1
		LED_1_Toggle(); x = 1;break;
		case 3: // Toggle Led 2
		LED_2_Toggle(); x = 2;break;
		case 4: // Toggle Led 3
		LED_3_Toggle(); x = 3;break;
		case 5: // Toggle Led 4
		LED_4_Toggle(); x = 4;break;
	}
	UART_Send_String_Polling_8("Led ");
	UART_Send_Byte_Polling_8(x);
	UART_Send_String_Polling_8("Toggled");
}