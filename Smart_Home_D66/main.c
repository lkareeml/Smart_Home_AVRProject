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
#include "MCAL/UART/UART.h"                              //Done
#include "MCAL/Analog_To_Digital_Converter/ADC.h"        //Done
#include "MCAL/TWI_I2C/TWI_I2C.h"                        //Done
#include "Application/Smart.h"                           //Done
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
/****************      Temperature Sensor Include   ********************/
#include "HAL/Temperature_Sensor/Temp_Sensor.h"     // NO CODE YET
/****************      Dimming Include   **************************/
#include "HAL/Dimming_Light/Dimmer.h"               //Done
/********************************************* **************************/

const uint8 Max_Failure_Login_Count = 3;

#define LED_Dimmer_Pin		0
#define LED_Dimmer_Port		PORTAx
#define Door_Servo_Pin		7
#define Door_Servo_Port		PORTDx
#define Admin_ID			99
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>


// ****************     UART VARIABLES     ****************

uint8 door_state = 0;// 0 is closed, 1 is open
uint8 G_Dimmer_Value = 0;
sint8 UART_Buffer[8];
uint8 FailCount = 0;
static uint8 UART_Received_Flag = 0;
static uint8 UART_global_Flag = 1;
static sint8 UART_UserID[8];
static uint8 User_ID;
static sint8 UART_Password[8];
static sint8 EEPROM_Username[8] = "wrong";
static sint8 EEPROM_Password[8] = "pass";
uint8 g_choice_1;
uint8 g_choice_2;
uint8 g_choice_2;
uint8 New_UserID;
sint8 New_Username[8];
sint8 New_Password[8];
static uint8 AC_State_Auto = 1; // Automatic 1 is on , 0 off allow manual
static uint8 AC_State = 0; // 0 means off 1 means on
const uint8 MAX_Users_Count = 5;// Without Admin

// ****************     KEYPAD LCD VARIABLES     ****************

uint8 LCD_g_step = 1;
uint8 LCD_UserInput = '\0';
uint8 LCD_user_count = 0;
uint8 LCD_pass_count = 0;
uint8 LCD_ID;
sint8 LCD_UserID[8];
sint8 LCD_Password[8];
sint8 LCD_EE_Password[8];
sint8 LCD_EE_Username[8];
uint8 LCD_Fail_Count = 0;
uint8 LCD_g_choice1 = 0;
uint8 LCD_g_choice2 = 0;

// **************************************************************




void Smart_Initialization(){
	LED_Init_All();
	LED_1_Off();
	LED_2_Off();
	LED_3_Off();
	LED_4_Off();
	LED_0_Off();
	LCD_Init();
	KEYPAD_Initialization();
	AC_Initialization();
	Servo_Init();
	Dimmer_Init();
	Dimmer_Off();
	EEPROM_Init();
	UART_Init();
	UART_Enable_RX_Interrupt();// PIE
	GIE_Enable(); // GIE
}

ISR(USART_RXC_vect){
	UART_Recieve_String_8(UART_Buffer);
	UART_Received_Flag = 1;
}
void Go_Back(){
	UART_Send_String_Polling_8("Going Back!\n");
	UART_Show_MainMenu();
	UART_global_Flag--;
}
void Go_Next(){
	UART_global_Flag++;
}

void UART_Process() {
	// step 1 user_id
	// step 2 user_password
	// step 3 choice_1(from main menu)
	// step 4 choice_2(from Control_Appliances/UserManagement/Settings)
	// step 5 choice_3(from ...)

	if (UART_Received_Flag == 1) {
		UART_Received_Flag = 0;

		switch (UART_global_Flag) {
			/************************* Login Process *************************/
			case 1: //Just Started, User Not loggedIn, user sent anything
			UART_Show_Request_UserID();
			Go_Next();
			break;
			case 2: // we got user input for userID now we validate it
			strcpy(UART_UserID, UART_Buffer); // Copy UART_Buffer To USERID
			UART_Show_Request_Password();
			Go_Next();
			break;
			case 3: // login Process
			strcpy(UART_Password, UART_Buffer); // Copy UART_Buffer to Password
			//			atoi ascii to integer || itoa integer to ascii
			User_ID = atoi(UART_UserID);
			//			Check if User Exists
			if (EEPROM_Read_UserID_Exist(User_ID)) {
				// Get Username and Password
				EEPROM_Read_8Data(User_ID, EEPROM_Username, 2);
				EEPROM_Read_8Data(User_ID, EEPROM_Password, 1);
			}
			// User and password Compare!
			if (strcmp(EEPROM_Password, UART_Password) == 0) {
				UART_Send_String_Polling_8("Welcome Back! ");
				UART_Send_String_Polling_8(EEPROM_Username);
				UART_Send_String_Polling_8("\n");
				UART_Show_MainMenu();
				Go_Next();
				} else {
				//				If Login Fail, Count Failure, Lock Sys
				FailCount++;
				if (FailCount >= Max_Failure_Login_Count) {
					UART_Send_String_Polling_8("Login Failed, \nSystem Locked! \n");
					BUZZER_ALARM_TILL_RESET();
				}
				UART_Show_Login_Failed();
				UART_global_Flag = 1;
			}
			break;
			/********************** End Login Process *************************/

			case 4: // Process choice 1 from MainMenu
			g_choice_1 = atoi(UART_Buffer);
			if (g_choice_1 <= 9 || g_choice_1 != 0) {
				// If user Input is valid BUT User is not admin
				// We Give him Admin Auth Refused
				// Admin Only: Control Door(3), Create,Delete(6,7)  User
				// Factory Reset(8)
				if ((g_choice_1 > 5 && g_choice_1 < 9) || (g_choice_1 == 3)) {
					if (User_ID != Admin_ID) {
						UART_Send_String_Polling_8("Auth Denied, Only Admin Function! \n");
						break;
					}
				}
				//Otherwise, Allow to handle every functionality
				UART_Choice_Handler_1(g_choice_1);
				// For Show user list No further Instructions Needed
					if (g_choice_1 != 5) {
						UART_global_Flag++;
					}else if(g_choice_1 == 8){
						UART_global_Flag = 2;
					}
				} else {
				//If user Input is Invalid
				UART_Show_Invalid();
				UART_Show_MainMenu();
			}
			break;

			case 5: // Process choice 2 from SubMenu
			g_choice_2 = atoi(UART_Buffer);
			if (g_choice_1 != 6 && g_choice_1 != 7) {
				if (g_choice_2 > 8) {
					UART_Show_Invalid();
					UART_Choice_Handler_1(g_choice_1);
					break;
				}
				if (g_choice_2 == 0) {
					UART_Show_MainMenu();
					UART_global_Flag--;
					break;
				}
			}

			switch (g_choice_1) {
				case 1: // LED Controller
				if (g_choice_2 < 6) {
					LED_Feedback(g_choice_2);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 2: // Air Conditioner Controller
				if (g_choice_2 < 4) {
					AC_Feedback(g_choice_2);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 3: // Door Controller
				if (g_choice_2 < 3) {
					Door_Feedback(g_choice_2);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 4: // Dimmer Controller
				if (g_choice_2 < 3) {
					Dimmer_Feedback(g_choice_2);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 6: // New User
				strcpy(New_Username, UART_Buffer);
				UART_Show_Request_NewUserPassword();
				UART_global_Flag++;
				break;
				case 7: // Delete User
				if (EEPROM_Read_UserID_Exist(g_choice_2)) {
					EEPROM_Delete_User(g_choice_2);
					UART_Send_String_Polling_8("User Deleted Successfully");
					} else {
					UART_Send_String_Polling_8("User Does not Exist!");
				}
				UART_global_Flag = 1;
				break;
			}
			break;
			case 6:
			strcpy(New_Password, UART_Buffer);
			New_UserID = EEPROM_Find_Empty_ID();
			if (New_UserID != 0) {
				EEPROM_Reg_New_User(New_UserID, New_Username, New_Password);
			}
			UART_Send_String_Polling_8("New User Registered Success!\n");
			UART_Send_String_Polling_8("New User ID: ");
			UART_Send_Byte_Polling_8(New_UserID + 48);
			UART_Send_String_Polling_8("\n");
			UART_Send_String_Polling_8("New User Username: ");
			UART_Send_String_Polling_8(New_Username);
			UART_Send_String_Polling_8("\n");
			UART_global_Flag = 1;
			break;
		}
	}
}



int main(void)
{
	Smart_Initialization();

	while (1) 
    {
		UART_Process();
		LCD_Process();
	}
}




void Smart_Configurator(){
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
	//User2 Initialize :
	sint8 User2_username[8] = "User2";
	sint8 User2_password[8] = "4321";
	uint8 User2_UserID = 2;
	EEPROM_Write_Byte(50,User2_UserID);
	EEPROM_Write_String(51,User2_password);
	EEPROM_Write_String(60,User2_username);		
	for(int i =1; i<1000;i++){
		EEPROM_Write_Byte(70+i,0);
	}
}