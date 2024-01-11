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
/****************      Dimming Include   **************************/
#include "HAL/Dimming_Light/Dimmer.h"               // NO CODE YET
/********************************************* **************************/

#define Max_Failure_Login_Count 3
#define LED_Dimmer_Pin		0
#define LED_Dimmer_Port		PORTAx
#define Door_Servo_Pin		7
#define Door_Servo_Port		PORTDx

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>


extern uint8 door_state;
extern uint8 G_Dimmer_Value;

sint8 Buffer[8];
static uint8 UART_Flag_Received = 0;
static uint8 global_Flag = 1;
static uint8 FailCount = 0;
static uint8 login_Fail = 1;
static sint8 UART_UserID[8];
static sint8 UART_Password[8];
static sint8 EEPROM_Username[8] = "wrong";
static sint8 EEPROM_Password[8] = "pass";
static sint8 UART_Choice_1[8];
static uint8 g_choice_1 = 0;
static sint8 UART_Choice_2[8];
static uint8 g_choice_2 = 0;
volatile static uint8 AC_State_Auto = 1; // Automatic 1 is on , 0 off allow manual
volatile static uint8 AC_State = 0; // 0 means off 1 means on
	
	
void Smart_Initialization(){
	LED_Init_All();
	LED_1_Off();LED_2_Off();LED_3_Off();LED_4_Off();LED_0_Off();
	AC_Initialization();
	Servo_Init();
	Dimmer_Init();Dimmer_Off();
	EEPROM_Init();
	UART_Init();
	UART_Enable_RX_Interrupt();// PIE
	GIE_Enable(); // GIE
}
ISR(USART_RXC_vect){
	UART_Recieve_String_8(Buffer);
	UART_Flag_Received = 1;
}
void Clear_Buffer(sint8 * Buffer){
	for(uint8 i = 0; i < sizeof(*Buffer); ++i) Buffer[i] = 0;
}
void Go_Back(){
	UART_Send_String_Polling_8("Going Back!\n");
	UART_Show_MainMenu();
	global_Flag--;
}

void Go_Next(){
	UART_Flag_Received = 0;
	Clear_Buffer(Buffer);
	global_Flag++;
}

void Go_To(uint8 Flag){
	UART_Flag_Received = 0;
	Clear_Buffer(Buffer);
	global_Flag = Flag;
}
void UART_Login_Failed(){
	UART_Send_String_Polling_8("Login Failed, Try again! ");
	UART_Send_Byte_Polling_8(FailCount + 48);
	UART_Send_String_Polling_8(" \n");
}

void UART_Process(){
	switch(global_Flag){
		case 1://Just Started, User Not loggedIn, user sent anything
			UART_Show_Request_UserID();
			Go_Next();
			break;
		case 2: // we got user input for userID now we validate it
			strcpy(UART_UserID,Buffer);// Copy Buffer To USERID
			UART_Show_Request_Password();
			Go_Next();
			break;
		case 3:
//				login Process
			strcpy(UART_Password,Buffer);// Copy Buffer to Password
//			atoi ascii to integer || itoa integer to ascii
			uint8 X_ID = atoi(UART_UserID);
			//Check if User Exists
			if(EEPROM_Read_UserID_Exist(X_ID)){
				// Get Username and Password
				EEPROM_Read_Data(X_ID,EEPROM_Username,2);
				EEPROM_Read_Data(X_ID,EEPROM_Password,1);				
			}
			// User and password Compare!
			if(strcmp(EEPROM_Password,UART_Password)==0){
				UART_Send_String_Polling_8("Welcome Back! ");
				UART_Send_String_Polling_8(EEPROM_Username);
				UART_Send_String_Polling_8("\n");
				UART_Show_MainMenu();
				login_Fail = 0;
				Go_Next();
			}else{
//				If Login Fail, Count Failure, Lock Sys
				FailCount++;
				if(FailCount >= Max_Failure_Login_Count){
					UART_Send_String_Polling_8("Login Failed, \nSystem Locked! \n");
					BUZZER_ALARM_TILL_RESET();
				}
				UART_Login_Failed();
				Go_To(1);
			}
			break;			
		case 4:
			// User Logged success, Process choice 1 from MainMenu
			strcpy(UART_Choice_1,Buffer);// Copy Buffer To Choice_1
			g_choice_1 = atoi(UART_Choice_1);
			if(g_choice_1 <= 9){
				UART_Choice_Handler_1(g_choice_1);
				Go_Next();
				break;
			}
			UART_Show_Invalid();
			UART_Show_MainMenu();
			Go_To(4);
			break;
		case 5:
			// User Logged success, Process choice 1 from MainMenu
			strcpy(UART_Choice_2,Buffer);// Copy Buffer To Choice_2
			UART_Flag_Received = 0;
			g_choice_2 = (UART_Choice_2[0]-48);
			//5,8,9 Invalid as no next step
			if(g_choice_1 == 6){
				// Create New User Handler 
				// Allow Only Admin To Create User!
				if(UART_UserID[0]-48 != 99){
					UART_Send_String_Polling_8("Auth Denied, Ask Admin! \n");
					break;//Break from Main Case 5
				}
			}
			
		if(g_choice_1 >= 5 || g_choice_2 > 5){
			UART_Show_Invalid();break;//Break from Main Case 5
		}
		// Handle Invalid Choice Out of range
		if( (g_choice_2 > 9 && g_choice_2 < 0) || (UART_Choice_2[1] != 0) ){
			UART_Show_Invalid();
			UART_Choice_Handler_1(g_choice_1);
			Clear_Buffer(UART_Choice_2);
			break;
		}
		switch(g_choice_1){
			case 1:
				// LED Controller
				switch(g_choice_2){
					case 1: // Toggle Led 0
					LED_0_Toggle();break;
					case 2: // Toggle Led 1
					LED_1_Toggle();break;
					case 3: // Toggle Led 2
					LED_2_Toggle();break;
					case 4: // Toggle Led 3
					LED_3_Toggle();break;
					case 5: // Toggle Led 4
					LED_4_Toggle();break;
					case 0: // Go Back
					Go_Back();break;
				}
				break;
			case 2:
				// Air Conditioner Controller
				if(g_choice_2 == 0) Go_Back();
				else AC_Feedback(g_choice_2);
				break;
			case 3:
				// Door Controller
// 				if(UART_UserID[0]-48 != 99){
// 					UART_Send_String_Polling_8("Auth Denied, Ask Admin! \n");
// 					break;//Break from Main Case 5
// 				}
				if(g_choice_2 == 0) Go_Back();
				else Door_Feedback(g_choice_2);
				break;
			case 4:
				//Dimmer Controller
				if(g_choice_2 == 0) Go_Back();
				else Dimmer_Feedback(g_choice_2);
				break;
		}
		//global_Flag++;
		break;
	}
}


int main(void)
{
	Smart_Initialization();
	
	//Initialize Default Users, Admin
	
	//Admin Initialize :
	sint8 Admin_username[8] = "Kareem\0";
	sint8 Admin_password[8] = "1234\0";
	uint8 Admin_UserID = 99;
	EEPROM_Write_Byte(10,Admin_UserID);
	EEPROM_Write_String(11,Admin_password);
	EEPROM_Write_String(20,Admin_username);
				
				
	while (1) 
    {
		if(UART_Flag_Received == 1){
			UART_Process();
		}
	}
}

