/*
 * Smart_Home_D66.c
 *
 * Created: 12/20/2023 3:23:04 AM
 * Author : Kareem
 */ 

/****************** Start Include Section **********************/
// Essentials:
#define F_CPU 16000000L
#include "MCAL/Digital_Input_Output/DIO.h"
#include "AVR32_Reg_Private.h"
#include "STD_Types.h"                                   //Done
#include "Bit_Manipulation.h"                            //Done
#include "MCAL/Interrupt/Interrupt.h"                    //Done
#include "MCAL/Timers/Timers.h"                          //Done
#include "MCAL/UART/UART.h"                              //NEEDS EDIT
#include "MCAL/Analog_To_Digital_Converter/ADC.h"        //Done
#include "MCAL/TWI_I2C/TWI_I2C.h"                        //Done
#include "Application/Smart.h"                                       //Done
/****************      Buzzer Include               ********************/
#include "HAL/Buzzer/Buzzer.h"                      //Done
/***************      KEYPAD Include               ********************/
#include "HAL/Keypad_4x4/Keypad_4x4.h"              //Done
/****************      AirConditioner Include       ********************/
#include "HAL/AirConditioner/AC.h"                  //Done
/****************      5 Leds Include               ********************/
#include "HAL/Leds_5/Leds.h"                        //Done
/****************      EEPROM Include               ********************/
#include "HAL/EEPROM/EEPROM.h"                      //Done
/****************      LCD Include                  ********************/
#include "HAL/LCD_16x2/LCD.h"                       //NEEDS EDIT
/****************      Door Servo Include           ********************/
#include "HAL/Door_Servo/Door_Servo.h"              //NEEDS EDIT
/****************      Bluetooth UART Include       ********************/
#include "HAL/Bluetooth/Bluetooth.h"                // NO CODE YET
/****************      Temperature Sensor Include   ********************/
#include "HAL/Temperature_Sensor/Temp_Sensor.h"     // NO CODE YET
/****************      Dimming Lamp Include   **************************/
#include "HAL/Dimming_Light/Dimmer.h"               // NO CODE YET
/********************************************* **************************/

#define Max_Failure_Login_Count 3
#define LED_Dimmer_Pin		0
#define LED_Dimmer_Port		PORTAx
#define Door_Servo_Pin		7
#define Door_Servo_Port		PORTDx
#define AC_Pin				2
#define AC_Port				PORTAx
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


	
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>




sint8 Buffer[8];
static uint8 UART_Flag_Received = 0;

void Smart_Initialization(){
	LED_Init_All();
	LED_1_Off();LED_2_Off();LED_3_Off();LED_4_Off();LED_0_Off();
	Lamp_Dimmer_Off();
	EEPROM_Init();
	UART_Init();
	GIE_Enable();
}
ISR(USART_RXC_vect){
	UART_Recieve_String_8(Buffer);
	UART_Flag_Received = 1;
}
void Clear_Buffer(sint8 * Buffer){
	for(uint8 i = 0; i < sizeof(*Buffer); ++i) Buffer[i] = 0;
}


void UART_Process(){
	static uint8 global_Flag = 1;
	static uint8 FailCount = 0;
	static sint8 UART_UserID[8];
	static sint8 UART_Password[8];
	static sint8 EEPROM_Username[8] = "Kareem";
	static sint8 EEPROM_Password[8] = "1234";
	static sint8 UART_Choice_1[8];
	static uint8 g_choice_1 = 0;
 	static sint8 UART_Choice_2[8];
	static uint8 g_choice_2 = 0;


	if(UART_Flag_Received == 1){
		switch(global_Flag){
			case 1://Just Started, User Not loggedIn, user sent anything
				UART_Show_Request_UserID();
				Clear_Buffer(Buffer);
				UART_Flag_Received = 0;
				global_Flag++;
				break;
			case 2: // we got user input for userID now we validate it
				strcpy(UART_UserID,Buffer);// Copy Buffer To USERID
				UART_Flag_Received = 0;
				UART_Show_Request_Password();
				global_Flag++;
				break;
			case 3:// Get password, Compare password in EEPROM, User login
				strcpy(UART_Password,Buffer);// Copy Buffer to Password
				UART_Flag_Received = 0;
				static uint8 login_Fail = 1;
			// ****************** I HAVE LOGICAL PROBLEM HERE!! ***************************
			//Test if User Does Exist and Password Exist
			// 					UART_UserID[1] = '\0'; // Protect from overflow if user input wrong input
			// 					sint8 username_test[8] = "Kareem";
			// 					EEPROM_Write_Byte(10,UART_UserID[0]);
			// 					EEPROM_Write_String(11,UART_Password);
			// 					EEPROM_Write_String(20,username_test);
			// 					if(EEPROM_Read_User_Password(UART_UserID[0],EEPROM_Username,EEPROM_Password) == 1){
			// 						//User and password Exist
			// 					}
			// ****************** END OF LOGICAL PROBLEM HERE!! ***************************
			// User and password Compare!
				if(strcmp(EEPROM_Password,UART_Password)==0){
					UART_Send_String_Polling_8("Welcome Back! ");
					UART_Send_String_Polling_8(EEPROM_Username);
					UART_Send_String_Polling_8("\n");
					UART_Show_MainMenu();
					login_Fail = 0;
					global_Flag++;
				}
			// If you Login Fail, Count Failure
				if(login_Fail == 1){
					FailCount++;
					if(FailCount >= Max_Failure_Login_Count){
						UART_Send_String_Polling_8("Login Failed, \nSystem Locked! \n");
						BUZZER_ALARM_TILL_RESET();
					}
					UART_Send_String_Polling_8("Login Failed, Try again! \n");
					UART_Send_Byte_Polling_8(FailCount + 48);
					UART_Send_String_Polling_8(" \n");
					global_Flag-=2;UART_Flag_Received = 1;
				}
				break;
			
			case 4:
			// User Logged success, Process choice 1 from MainMenu
				strcpy(UART_Choice_1,Buffer);// Copy Buffer To Choice_1
				UART_Flag_Received = 0;
				g_choice_1 = (UART_Choice_1[0]-48);
				if( (g_choice_1 > 9 && g_choice_1 < 1) || (UART_Choice_1[1] != 0) ){
					UART_Show_Invalid();
					UART_Show_MainMenu();
					Clear_Buffer(UART_Choice_1);
					break;
				}
				UART_Choice_Handler_1(g_choice_1);
				global_Flag++;
				break;
		case 5:
			// User Logged success, Process choice 1 from MainMenu
			strcpy(UART_Choice_2,Buffer);// Copy Buffer To Choice_2
			UART_Flag_Received = 0;
			g_choice_2 = (UART_Choice_2[0]-48);
			//5,8,9 Invalid as no next step
			
			if(g_choice_1 == 6){// Create New User Handler 
				// Allow Only Admin To Create User!
				if(UART_UserID[0]-48 != 99){
					UART_Send_String_Polling_8("Auth Denied, Ask Admin! \n");
					break;//Break from Main Case 5
				}
			}
			if(g_choice_1 == 5 || g_choice_1 > 6){
				UART_Show_Invalid();break;//Break from Main Case 5
			}
			// Handle Invalid Choice Out of range
			if( (g_choice_2 > 9 && g_choice_2 < 0) || (UART_Choice_2[1] != 0) ){
				UART_Show_Invalid();
				UART_Choice_Handler_1(g_choice_1);
				Clear_Buffer(UART_Choice_2);
				break;
			}
			

						
			// LED Controller
			if(g_choice_1 == 1){
				if(g_choice_2 > 5){
					UART_Show_Invalid();break;//Break from Main Case 5
				}
				switch(g_choice_2){
					case 1: // Toggle Led 0
						DIO_Set_Pin_Output(Led0_Port,Led0_Pin,Toggle);break;
					case 2: // Toggle Led 1
						DIO_Set_Pin_Output(Led1_Port,Led1_Pin,Toggle);break;
					case 3: // Toggle Led 2
						DIO_Set_Pin_Output(Led2_Port,Led2_Pin,Toggle);break;
					case 4: // Toggle Led 3
						DIO_Set_Pin_Output(Led3_Port,Led3_Pin,Toggle);break;
					case 5: // Toggle Led 4
						DIO_Set_Pin_Output(Led4_Port,Led4_Pin,Toggle);break;
					case 0: // Go Back
						UART_Send_String_Polling_8("Going Back!\n");
						UART_Show_MainMenu();
						global_Flag--;
					break;
				}
			}
			// Air Conditioner Controller
			else if(g_choice_1 == 2){
				if(g_choice_2 > 3){
					UART_Show_Invalid();break;//Break from Main Case 5
				}
				switch(g_choice_2){
					case 1: // AC Auto
					break;
					case 2: // AC Manual Turn OFf
					break;
					case 3: // AC Manual Turn On
					break;
					case 0: // Go Back
					break;
				}
			}
			// Door Controller
			else if(g_choice_1 == 3){
				if(UART_UserID[0]-48 != 99){
					UART_Send_String_Polling_8("Auth Denied, Ask Admin! \n");
					break;//Break from Main Case 5
				}
				else if(g_choice_2 > 2){
					UART_Show_Invalid();break;//Break from Main Case 5
				}
				switch(g_choice_2){
					case 1: // Open Door
					break;
					case 2: // Close Door
					break;
					case 0: // Go Back
					break;
				}
			}
			//Dimmer Controller
			else if(g_choice_1 == 4){
				if(g_choice_2 > 4){
					UART_Show_Invalid();break;//Break from Main Case 5
				}
				switch(g_choice_2){
					case 1: // Dimmer Up
					break;
					case 2: // Dimmer Down
					break;
					case 3: // Dimmer Off
					break;
					case 4: // Dimmer On
					break;
					case 0: // Go Back
					break;
				}
			}
			//global_Flag++;
			break;
		}
	}
}


int main(void)
{
	UART_Enable_RX_Interrupt();
	Smart_Initialization();
	DIO_Set_Pin_Output(PORTCx,7,High);
	DIO_Set_Pin_Output(PORTDx,3,High);
	DIO_Set_Pin_Output(PORTCx,2,High);
   
	while (1) 
    {
		UART_Process();
	}
}

