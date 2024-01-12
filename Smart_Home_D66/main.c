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
#include "HAL/LCD_16x2/LCD.h"                       //Done
/****************      Door Servo Include           ********************/
#include "HAL/Door_Servo/Door_Servo.h"              //Done
/****************      Bluetooth UART Include       ********************/
#include "HAL/Bluetooth/Bluetooth.h"                // NO CODE YET
/****************      Temperature Sensor Include   ********************/
#include "HAL/Temperature_Sensor/Temp_Sensor.h"     // NO CODE YET
/****************      Dimming Include   **************************/
#include "HAL/Dimming_Light/Dimmer.h"               //Done
/********************************************* **************************/

#define Max_Failure_Login_Count 3
#define LED_Dimmer_Pin		0
#define LED_Dimmer_Port		PORTAx
#define Door_Servo_Pin		7
#define Door_Servo_Port		PORTDx
#define Admin_ID			99
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

extern uint8 door_state;
extern uint8 G_Dimmer_Value;

sint8 Buffer[8];
static uint8 UART_Flag_Received = 0;
static uint8 global_Flag = 3;
static uint8 FailCount = 0;
static sint8 UART_UserID[8] = "99";
static uint8 User_ID = 99;
static sint8 UART_Password[8];
static sint8 EEPROM_Username[8] = "wrong";
static sint8 EEPROM_Password[8] = "pass";
uint8 g_choice_1;
uint8 g_choice_2;
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
void Clear_Token(){
	UART_Flag_Received = 0;
}
void Go_Back(){
	UART_Send_String_Polling_8("Going Back!\n");
	UART_Show_MainMenu();
	global_Flag--;
}
void Go_Next(){
	Clear_Token();
	global_Flag++;
}

void Go_To(uint8 Flag){
	Clear_Token();
	global_Flag = Flag;
}
void UART_Login_Failed(){
	UART_Send_String_Polling_8("Login Failed, Try again! ");
	UART_Send_Byte_Polling_8(FailCount + 48);
	UART_Send_String_Polling_8(" \n");
}

void UART_Process(){
	if(UART_Flag_Received == 1){
		UART_Flag_Received = 0;
		
	switch(global_Flag){
		/************************* Login Process *************************/
		case 1://Just Started, User Not loggedIn, user sent anything
			UART_Show_Request_UserID();Go_Next();break;
		case 2: // we got user input for userID now we validate it
			strcpy(UART_UserID,Buffer);// Copy Buffer To USERID
			UART_Show_Request_Password();Go_Next();break;
		case 3:// login Process
			strcpy(UART_Password,Buffer);// Copy Buffer to Password
//			atoi ascii to integer || itoa integer to ascii
			User_ID = atoi(UART_UserID);
//			Check if User Exists
			if(EEPROM_Read_UserID_Exist(User_ID)){
				// Get Username and Password
				EEPROM_Read_Data(User_ID,EEPROM_Username,2);
				EEPROM_Read_Data(User_ID,EEPROM_Password,1);				
			}
			// User and password Compare!
			if(strcmp(EEPROM_Password,UART_Password)==0){
				UART_Send_String_Polling_8("Welcome Back! ");
				UART_Send_String_Polling_8(EEPROM_Username);
				UART_Send_String_Polling_8("\n");
				UART_Show_MainMenu();
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
			
		/********************** End Login Process *************************/
		case 4:// Process choice 1 from MainMenu
			g_choice_1 = atoi(Buffer);
			if(g_choice_1 <= 9 || g_choice_1 != 0){
				UART_Choice_Handler_1(g_choice_1);
				Clear_Token();global_Flag++;
			}else{
				UART_Show_Invalid();
				UART_Show_MainMenu();
				Clear_Token();
			}
			break;
			
		case 5:// Process choice 2 from SubMenu
			g_choice_2 = atoi(Buffer);
			if(g_choice_2 > 9){
				UART_Show_Invalid();
				UART_Choice_Handler_1(g_choice_1);
				break;
			}
			if(g_choice_2 == 0) {
				UART_Show_MainMenu();
				global_Flag--;
				break;
			}
			switch(g_choice_1){
				case 1:// LED Controller
					if(g_choice_2 < 6)
						UART_LED_Feedback(g_choice_2);
					break;
				case 2:// Air Conditioner Controller
					AC_Feedback(g_choice_2);break;
				case 3:// Door Controller
					if(User_ID != Admin_ID){
						UART_Send_String_Polling_8("Auth Denied, Ask Admin! \n");
						break;
					}else{
						Door_Feedback(g_choice_2);break;
					}
				case 4:// Dimmer Controller
					Dimmer_Feedback(g_choice_2);break;
			}
			break;
	}
}
}


int main(void)
{
	Smart_Initialization();

	sint8 Admin_username[8] = "Kareem";
	sint8 Admin_password[8] = "1234";
	EEPROM_Write_Byte(10,Admin_ID);
	EEPROM_Write_String(11,Admin_password);
	EEPROM_Write_String(20,Admin_username);
	//User1 Initialize :
	sint8 User1_username[8] = "User1";
	sint8 User1_password[8] = "4321";
	uint8 User1_UserID = 1;
	EEPROM_Write_Byte(30,User1_UserID);
	EEPROM_Write_String(31,User1_password);
	EEPROM_Write_String(40,User1_username);
			
	while (1) 
    {
		UART_Process();
	}
}

