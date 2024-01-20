/*	AMIT Embedded System Diploma 
Batch: D66

Smart Home Application Made by: Kareem Atef
Here we will document every important part in the application
Starting with Pin Needed Calculation To choose micro controller:

******************************************************************************
--- Peripheral Name -- No.Pins -- Pin.Name ------------ Com.Type
******************************************************************************
******************* For Communication And Storage ****************************
    Bluetooth          2 Pin      PD0 PD1               (UART) {TX-RX} 
    External EEPROM    2 Pin      PC0 PC1               (I2C)  {SCL-SDA} 
******************* For User Input *******************************************
    Temp_Sensor/POT    1 Pin      PA1
    Keypad(4X4)        8 Pin      PB4..PB7, PC3..PC6
******************* For Output ***********************************************
    LCD(16X2)          7 Pin      PA4..PA7, PB1..PB3
    Buzzer             1 Pin      PA3
******************* For Controllable Appliances ******************************
    Door_Servo         1 Pin      PD7
    5-Lamp             5 Pin      PC2 PC7 PD3 PD4 PD5
    Dimming Lamp       1 Pin      PA0
    Air Conditioner    1 Pin      PA2
******************************************************************************

Timers:
Timer1 >> For Servo and Door State
Timer0 >> Dimmer

******************************************************************************
Some Notes: 

Login system admin and user “admin is remoted only.
--->    Admin mode can register Create / Delete Users for Admin Only{EEPROM}
--->    Usernames and passwords kept in EEPROM
--->    user/passwords wrong 3 trial,alarm until reset.
--->    door Control for Admin Only
--->    USER control even any user login remote, except admin until allowing of admin
--->    LCD must display (IDLE)
--->    user’s usernames LCD different user’s usernames in the UART system
--->    Temperature Sensor if temp > 28 °C, Air condition TURN_ON DC_Motor
--->    Temperature Sensor if temp < 21 °C, Air condition    TURN_OFF DC_Motor
*/

#define F_CPU 16000000L

/****************** Start Include Section **********************/
// Essentials:
#include "../STD_Types.h"                                   //Done
#include "../Bit_Manipulation.h"                            //Done
#include "../AVR32_Reg_Private.h"							//Done
#include "../MCAL/Digital_Input_Output/DIO.h"				//Done
#include "../MCAL/Interrupt/Interrupt.h"                    //Done
#include "../MCAL/Timers/Timers.h"                          //Done
#include "../MCAL/UART/UART.h"                              //Done
#include "../MCAL/Analog_To_Digital_Converter/ADC.h"        //Done
#include "../MCAL/TWI_I2C/TWI_I2C.h"                        //Done
#include "../MCAL/Interrupt/Interrupt.h"                    //Done
#include "../MCAL/Timers/Timers.h"                          //Done
#include "../MCAL/UART/UART.h"                              //Done
#include "../MCAL/Analog_To_Digital_Converter/ADC.h"        //Done
#include "../HAL/Buzzer/Buzzer.h"							//Done
#include "../HAL/Keypad_4x4/Keypad_4x4.h"					//Done
#include "../HAL/AirConditioner/AC.h"						//Done
#include "../HAL/Leds_5/Leds.h"								//Done
#include "../HAL/EEPROM/EEPROM.h"							//Done
#include "../HAL/LCD_16x2/LCD.h"							//Done
#include "../HAL/Door_Servo/Door_Servo.h"					//Done
#include "../HAL/Temperature_Sensor/Temp_Sensor.h"			// NO CODE YET
#include "../HAL/Dimming_Light/Dimmer.h"					//Done
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "../Global_Var.h"

void UART_Process();
void LCD_Process();
void LCD_Idle();
void AC_Auto_Idle();

/****************    Configuration        ******************/

#define Servo_Port	PORTDx
#define Servo_Pin	Pin7
#define Admin_ID	1
uint8 Max_Failure_Login_Count = 3;
uint8 MAX_Users_Count = 5;// Without Admin
uint8 AC_Auto_Threshold_Up = 28;
uint8 AC_Auto_Threshold_Down = 21;

// ****************     UART VARIABLES     ****************

uint8 door_state = 0;// 0 is closed, 1 is open
uint8 G_Dimmer_Value = 0; // For Dimmer Brightness
sint8 UART_Buffer[8];
uint8 FailCount = 0;
uint8 UART_Received_Flag = 0;
uint8 UART_global_Flag = 1;
sint8 UART_UserID[8];
uint8 User_ID;
sint8 UART_Password[8];
sint8 EEPROM_Username[8];
sint8 EEPROM_Password[8];
uint8 UART_G_Choice_1;
uint8 UART_G_Choice_2;
uint8 UART_G_Choice_3;
uint8 New_UserID;
sint8 New_Username[8];
sint8 New_Password[8];

//uint8 AC_State_Auto = 1; // Automatic 1 is on , 0 off allow manual

uint8 AC_Auto = 1; // Auto when Start 0 = Manual,  1 = Auto
uint8 AC_State = 0; // Auto when Start 0 = off,  1 = on
uint8 g_Temp = 29;
uint8 Old_g_Temp;
uint8 Temp_Changed = 1;
// ****************     KEYPAD LCD VARIABLES     ****************

uint8 LCD_g_step = 0;				//Start at Idle Step
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
//uint16 LCD_Idle_Stop = 0;

// **************************************************************

// UART RX Complete Interrupt, get user input immediately save it to buffer
// Then change receiving flag to 1 indicate UART Received Buffer
ISR(USART_RXC_vect){
	UART_Recieve_String_8(UART_Buffer);
	UART_Received_Flag = 1;
}

//Dimmer ISR(Timer0)

//Door Servo ISR (Timer1 Compare Match) 
ISR(TIMER1_COMPA_vect){
	static uint8 counter = 0;
	switch(counter){
		case 0:
			DIO_Set_Pin_Output(Servo_Port,Servo_Pin,High);
			counter++;
			break;
		case 1:
			if(door_state == 0){
				DIO_Set_Pin_Output(Servo_Port,Servo_Pin,Low);
			}
			counter++;
			break;
		case 5:
			if(door_state == 1){
				DIO_Set_Pin_Output(Servo_Port,Servo_Pin,Low);
			}
			counter++;
			break;
		case (40):
			counter = 0;
			break;
		default:
			counter++;
			break;
	}
}

void Smart_Initialization(){
	LED_Init_All();
	LED_1_Off();
	LED_2_Off();
	LED_3_Off();
	LED_4_Off();
	LED_0_Off();
	ADC_Init();		// Enable ADC to measure Temperature
	Dimmer_Init();	//
	KEYPAD_Initialization();
	LCD_Init();
	AC_Initialization();
	Servo_Init();
	EEPROM_Init();
	UART_Init();
	UART_Enable_RX_Interrupt();// PIE
	GIE_Enable(); // GIE
}

void ADC_Temperature_Update(){
	
	g_Temp = ADC_Convert_8bit(ADC_Ch1) * 50 / 256; //0 -- 50 c
	if((g_Temp - Old_g_Temp) !=0){
		Temp_Changed = 1;
		Old_g_Temp = g_Temp;
	}
	//LCD_Idle_Stop = 0;
}

void UART_Process() {
	if (UART_Received_Flag == 1) {
		UART_Received_Flag = 0;
		switch (UART_global_Flag) {
			/************************* Login Process *************************/
			case 1: //Just Started, User Not loggedIn, user sent anything
			UART_Show_Request_UserID();
			UART_global_Flag++;
			break;
			case 2: // we got user input for userID now we validate it
			strcpy(UART_UserID, UART_Buffer); // Copy UART_Buffer To USERID
			UART_Show_Request_Password();
			UART_global_Flag++;
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
				UART_global_Flag++;
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
			UART_G_Choice_1 = atoi(UART_Buffer);
			if (UART_G_Choice_1 <= 9 || UART_G_Choice_1 != 0) {
				// If user Input is valid, BUT User is not admin
				// Reply Admin Auth Required
				// Admin Only: Control Door(3), Create,Delete(6,7)
				if ((UART_G_Choice_1 > 5 && UART_G_Choice_1 < 9) || (UART_G_Choice_1 == 3)) {
					if (User_ID != Admin_ID) {
						UART_Send_String_Polling_8("Auth Denied, Only Admin Function! \n");
						break;
					}
				}
				//Otherwise, Allow to handle every functionality
				UART_Choice_Handler_1(UART_G_Choice_1);
				// For Show user list No further Instructions Needed
				if (UART_G_Choice_1 != 5) {
					UART_global_Flag++;
					}else if(UART_G_Choice_1 == 8){
					UART_global_Flag = 2;
				}
				} else {
				//If user Input is Invalid
				UART_Show_Invalid();
				UART_Show_MainMenu();
			}
			break;

			case 5: // Process choice 2 from SubMenu
			UART_G_Choice_2 = atoi(UART_Buffer);
			if (UART_G_Choice_1 != 6 && UART_G_Choice_1 != 7) {
				if (UART_G_Choice_2 > 8) {
					UART_Show_Invalid();
					UART_Choice_Handler_1(UART_G_Choice_1);
					break;
				}
				if (UART_G_Choice_2 == 0) {
					UART_Show_MainMenu();
					UART_global_Flag--;
					break;
				}
			}

			switch (UART_G_Choice_1) {
				case 1: // LED Controller
				if (UART_G_Choice_2 < 6) {
					LED_Feedback(UART_G_Choice_2,UART);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 2: // Air Conditioner Controller
				if (UART_G_Choice_2 < 4) {
					AC_Feedback(UART_G_Choice_2,UART);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 3: // Door Controller
				if (UART_G_Choice_2 < 3) {
					Door_Feedback(UART_G_Choice_2,UART);
					} else {
					UART_Show_Invalid();
				}
				break;
				case 4: // Dimmer Controller
				if (UART_G_Choice_2 < 3) {
					Dimmer_FeedBack(UART_G_Choice_2,UART);
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
				if (EEPROM_Read_UserID_Exist(UART_G_Choice_2)) {
					EEPROM_Delete_User(UART_G_Choice_2);
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

void Smart_Configurator(){
	sint8 Admin_username[8] = "Kareem";
	sint8 Admin_password[8] = "1234";
	EEPROM_Write_Byte(30,Admin_ID);
	EEPROM_Write_String(31,Admin_password);
	EEPROM_Write_String(40,Admin_username);
	//User1 Initialize :
	sint8 User1_username[8] = "User1";
	sint8 User1_password[8] = "4321";
	uint8 User1_UserID = 2;
	EEPROM_Write_Byte(50,User1_UserID);
	EEPROM_Write_String(51,User1_password);
	EEPROM_Write_String(60,User1_username);
	//User2 Initialize :
	sint8 User2_username[8] = "User2";
	sint8 User2_password[8] = "4321";
	uint8 User2_UserID = 3;
	EEPROM_Write_Byte(70,User2_UserID);
	EEPROM_Write_String(71,User2_password);
	EEPROM_Write_String(80,User2_username);
// 	for(int i =1; i<1000;i++){
// 		EEPROM_Write_Byte(70+i,0);
// 	}
}

void LCD_Show(uint8 stage){
	LCD_Send_Clear_Screen();
	switch(stage){
		case 1:// Get_UserID
			LCD_Send_String("Enter UserID:   ");break;
		case 2:// Get_Password
			LCD_Send_String("Enter Password: ");break;
		case 3:// Welcome_User
			LCD_Send_String("Welcome Back!   ");break;
		case 4:// Auth Failed
			LCD_Send_String("Auth Failed!    ");break;
		case 5:// Invalid Selected
			LCD_Send_String("Invalid Selected");
			LCD_Send_String("Try Again       ");break;
		case 6://Main_Options
			LCD_Send_String("(1)AC  (2)Dimmer");
			LCD_Send_String("(3)Led (4)Logout");break;
		case 7://AC_Options
			LCD_Send_String("AC(1)Auto (2)Off");
			LCD_Send_String("AC(3)ON (0)Back ");break;
		case 8://Dimmer_Options
			LCD_Send_String("Dimmer Higher(1)");
			LCD_Send_String("(2)Lower (0)Back");break;
		case 9://LED_Options
			LCD_Send_String("Led No.(1)(2)(3)");
			LCD_Send_String("(4)(5)   (0)Back");break;
		case 10:// System Locked
			LCD_Send_String("Auth Failed!    ");
			LCD_Send_String("System Locked   ");break;
		case 11: LCD_Send_String("Done!");break;
	}
}


void LCD_MEM_Clear(){
	LCD_user_count = 0;
	LCD_pass_count = 0;
	LCD_ID = 0;
	for(uint8 i =0;i<8;i++){
		LCD_UserID[i] = 0;
		LCD_Password[i] = 0;
		LCD_EE_Password[i] = 0;
		LCD_EE_Username[i] = 0;
	}
}

void LCD_Process(){
	switch(LCD_g_step){
		case 0:
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			else{
				LCD_UserInput = '\0';
				LCD_g_step++;
			}
			break;
		case 1:
			LCD_MEM_Clear();
			LCD_Show(1);// Show Request UserID
			LCD_g_step++;
			break;
		case 2:
			// Get user ID
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput >= '0' && LCD_UserInput <= '9'){
				LCD_Send_Char(LCD_UserInput);
				LCD_UserID[LCD_user_count] = LCD_UserInput;
				LCD_user_count++;
			}else if (LCD_UserInput == '*' && LCD_user_count > 0){
				LCD_Delete_Last_Written();
				LCD_UserID[LCD_user_count] = '\0';
				LCD_user_count--;
			}else if (LCD_UserInput >= 'A' && LCD_UserInput <= 'D') {
				LCD_g_step++;
			}
			LCD_UserInput = '\0';
			break;
		case 3:
			LCD_Show(2);// Show Request Password
			LCD_user_count = 0;
			LCD_g_step++;
			break;
		case 4:
			// Get user Password
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput >= '0' && LCD_UserInput <= '9'){
				LCD_Send_Char(LCD_UserInput);
				LCD_Password[LCD_pass_count] = LCD_UserInput;
				LCD_pass_count++;
			}
			else if (LCD_UserInput == '*' && LCD_pass_count > 0){
				LCD_Delete_Last_Written();
				LCD_Password[LCD_pass_count] = '\0';
				LCD_pass_count--;
			}
			else if (LCD_UserInput >= 'A' && LCD_UserInput <= 'D') {
				LCD_g_step++;
			}
			LCD_UserInput = '\0';
			break;
			
		case 5: 
			//User/Password Login Process
			// If User Exist, Fetch Password
			LCD_ID = atoi(LCD_UserID);
			if(EEPROM_Read_UserID_Exist(LCD_ID)){
				EEPROM_Read_8Data(LCD_ID,LCD_EE_Password,1);
			}
			//If password Match, Login Success!
			if(strcmp(LCD_Password,LCD_EE_Password) == 0){
				EEPROM_Read_8Data(LCD_ID,LCD_EE_Username,2);
				LCD_Show(3);
				LCD_Send_String(LCD_EE_Username);
				LCD_g_step++;
			}else{
				//Else Login Failed, Increase Fail Counter
				LCD_Show(4);// Auth Failed
				LCD_Fail_Count++;
				LCD_g_step = 1;
				if(LCD_Fail_Count == Max_Failure_Login_Count){
					UART_Send_String_Polling_8("\nKEYPAD Login Failed, \nSystem Locked! \n");
					LCD_Show(10);// System Locked
					BUZZER_ALARM_TILL_RESET();
				}
			}
			break;
			
			
		case 6:
			//_delay_ms(1000);
			LCD_Show(6);// Main_Options
			LCD_g_step++;
			break;
		case 7:
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput > '0' && LCD_UserInput <= '4'){
				LCD_g_choice1 = LCD_UserInput - 48;
				LCD_UserInput = '\0';
				LCD_g_step++;
			}else{
				LCD_Show(5);// Invalid Selected
				LCD_g_step--;
			}
			break;

		case 8:
			LCD_g_step++;
			switch(LCD_g_choice1){
				case 1: LCD_Show(7);break;//AC_Options
				case 2: LCD_Show(8);break;//Dimmer_Options
				case 3: LCD_Show(9);break;//LED_Options
				case 4: 
					LCD_Send_Clear_Screen();
					LCD_g_step = 0;//Go to LCD IDLE
					Temp_Changed = 1;//Make Temp Changed To enable display First time
					break;//Logout
			}
			break;
		case 9:
			LCD_UserInput = KEYPAD_Get_Pressed_Key();
			if (LCD_UserInput == '\0') break;
			if (LCD_UserInput >= '0' && LCD_UserInput <= '5'){
				LCD_g_choice2 = LCD_UserInput - 48;
				LCD_UserInput = '\0';
				LCD_g_step++;
				}else{
				LCD_Show(5);// Invalid Selected
				LCD_g_step--;
			}
			break;
		case 10:
			switch(LCD_g_choice1){
				case 1:
					if(LCD_g_choice2 == 0){
						LCD_g_step-= 2;break;
					}
					else if(LCD_g_choice2 < 4) {
						AC_Feedback(LCD_g_choice2,LCD);
					}
					else{
						LCD_g_step-=2;
					}
					break;
				case 2:
					if(LCD_g_choice2 == 0){
						LCD_g_step-= 2;break;
					}
					else if(LCD_g_choice2 < 3){
						 Dimmer_FeedBack(LCD_g_choice2,LCD);
					}else{
						LCD_g_step-=2;
					}
					break;
				case 3:
					if(LCD_g_choice2 == 0){
						LCD_g_step-= 2;break;
					}
					else if(LCD_g_choice2 < 6) {
						LED_Feedback(LCD_g_choice2,LCD);
					}else{
						LCD_g_step-=2;
					}
					break;
			}
			//LCD_Show(11);
			LCD_g_step-=2;
			break;
	}
}

void LCD_Idle(){
//		[AC(ON) Temp(30c)]
//		[        (0) More]
//		Counter to limit the number of Running This function
// 		LCD_Idle_Stop++;
// 		if(LCD_Idle_Stop > 7900){
// 			LCD_Idle_Stop = 0;
// 		}
	if(LCD_g_step != 0 || Temp_Changed == 0) return;
	if(LCD_g_step == 0 && Temp_Changed == 1){
		Temp_Changed = 0;
		//LCD_Send_Clear_Screen(); // Clears anything on screen
		sint8 snum[3];
		itoa(g_Temp, snum, 10);
		LCD_Send_String("AC(O"); // AC(O as it can be On or Off
		if     (AC_State == 0) LCD_Send_String("FF)");
		else if(AC_State == 1) LCD_Send_String("N) ");
		LCD_Send_String("Temp(");
		LCD_Send_String(snum);
		LCD_Send_String("c)");
		if     (g_Temp <  10) LCD_Send_String(" ");
		LCD_Send_String("(0) More        ");
		// To prevent screen Reloading So fast
	}
}

void AC_Auto_Idle(){
	// If AC is manual not Auto Return
	if(AC_Auto != 1) return;
	//When AC set To Auto Mode
	if(AC_Auto == 1){
		//When Temperature is Higher than Hot
		if(g_Temp >= AC_Auto_Threshold_Up){
			AC_On();
		}else if(g_Temp <= AC_Auto_Threshold_Down){//When Temperature is Lower than Cold
			AC_Off();
		}
	}
}

void Smart_Idle(){
	
	// There are 6 Functions should always work at idle
	// 1) UART Bluetooth (Interrupt)
		UART_Process();
	// 2) The Temperature (ADC Polling Prescaler 2 Fast)
		ADC_Temperature_Update();
	// 3) AirConditioner Automatic		
		AC_Auto_Idle();
	// 4) LCD & Keypad system
		LCD_Process();
	// 5) LCD Idle
		LCD_Idle();
	// 6) The door Servo				(Timer1 + Interrupt)
	// 7) The Dimmer					(Timer0 + Interrupt)

}
