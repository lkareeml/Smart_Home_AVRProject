/*

AMIT Embedded System Diploma 
Batch: D66

Smart Home Application Made by Team Members: ...  
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

Total Pins Needed for the Project : 29

Calculation of Timers Needed : 
Calculation of ADC Needed :

Any other Needed Technology for the project ?


Suitable Microcontroller : 


Timers:
Timer1 >> For Servo and Door State

Timer0 >>

Timer2 >> 

******************************************************************************
******************************************************************************
******************************************************************************
Some Notes: 

Login system admin and user “admin is remoted only.
--->    Admin mode can register any user or remove.
--->    Usernames and password must be kept into memory even at powered off. .... EEPROM
--->    If any user/admin/passwords wrong more than 3 trial, system must break down and fire alarm until reset.
--->    Admin and user can access to all applies except user cant control the door opening

// Specifications LCD & keypad USER MODE ONLY
        Normal user control all except opening door
        USER MODE control system even if any user login remotely except admin until allowing of admin
        LCD must display running devices if keypad-LCD (IDLE)
        user’s usernames in this system different than the user’s usernames in the remoting system

// Specifications EEPROM
        Store new user /registration/
        ADMIN_MODE Read/write 
        USER_MODE read only

// Specifications Bluetooth
        Transmitting/Receiving the commands to run the system.

// Specifications Lamps, Relay, Dimmer
        Lamps must be RELAY isolated because of high power.
        Dimmer circuit control lamp depending

// Specifications Door
        ADMIN_MODE ONLY can control SERVO_MOTOR to control door

// Specifications –Temperature Sensor, DC motor
        Temperature Sensor if temp > 28 °C, Air condition TURN_ON DC_Motor
        Temperature Sensor if temp < 21 °C, Air condition    TURN_OFF DC_Motor

// Components needed :
        External EEPROM
        Bluetooth module HC-05
        5 LEDs
        1 dimming circuit.
        LM35 temperature sensor or equivalent//Using POT ON AMIT KIT
        DC motor/ OR AC/ Or Relay
        Keypad 
        Lm01602A Character LCD.
        Servo motor
*/


/****************** Start Include Section **********************/
// Essentials:
#include "../STD_Types.h"                                   //Done
#include "../Bit_Manipulation.h"                            //Done
#include "../MCAL/Interrupt/Interrupt.h"                    //Done
#include "../MCAL/Timers/Timers.h"                          //Done
#include "../MCAL/UART/UART.h"                              //NEEDS EDIT
#include "../MCAL/Analog_To_Digital_Converter/ADC.h"        //Done
#include "../MCAL/TWI_I2C/TWI_I2C.h"                        //Done
#include "Smart.h"                                          //Done
#include <stdlib.h>
#include <string.h>

/****************      Buzzer Include               ********************/
#include "../HAL/Buzzer/Buzzer.h"                      //Done
/****************      KEYPAD Include               ********************/
#include "../HAL/Keypad_4x4/Keypad_4x4.h"              //Done
/****************      AirConditioner Include       ********************/
#include "../HAL/AirConditioner/AC.h"                  //Done
/****************      5 Leds Include               ********************/
#include "../HAL/Leds_5/Leds.h"                        //Done
/****************      EEPROM Include               ********************/
#include "../HAL/EEPROM/EEPROM.h"                      //NEEDS EDIT
/****************      LCD Include                  ********************/
#include "../HAL/LCD_16x2/LCD.h"                       //NEEDS EDIT
/****************      Door Servo Include           ********************/
#include "../HAL/Door_Servo/Door_Servo.h"              //NEEDS EDIT
/****************      Bluetooth UART Include       ********************/
#include "../HAL/Bluetooth/Bluetooth.h"                // NO CODE YET
/****************      Temperature Sensor Include   ********************/
#include "../HAL/Temperature_Sensor/Temp_Sensor.h"     // NO CODE YET
/****************      Dimming Lamp Include   **************************/
#include "../HAL/Dimming_Light/Dimmer.h"               // NO CODE YET
/********************************************* **************************/

void Smart_Initialization(){
    
}

uint8 String_Compare(sint8* str1,sint8* str2){
	//return 0 if mismatch
	//return 1 if match
	if(strlen(str1) != strlen(str2)) return 0; // Mismatch Due to size
	for(int i = 0;i<=strlen(str1);i++){
		if(str1[i] == str2[i]) continue;
		else return 0;// Mismatch
	}
	return 1; // Match each char
}
void Appliances_Controller(State New_State,Device Appliance){
	switch(Appliance){
		case Door_Lock_Servo://Servo on PD7
		if(New_State == Turn_On) Servo_180_Degree_Delay();///EDIT
		else if(New_State == Turn_Off) Servo_0_Degree_Delay();///EDIT
		break;
		case Dim_Lamp_Device://PA0
		if(New_State == Turn_On) Lamp_Dimmer_On();
		else if(New_State == Turn_Off) Lamp_Dimmer_Off();
		break;
		case Air_Conditioner_Device://AC ON RELAY ON PA2
		if(New_State == Turn_On) AC_On();
		else if(New_State == Turn_Off) AC_Off();
		break;
		case LED_0_Device://LED0 ON KIT is PC2
		if(New_State == Turn_On) LED_0_On();
		else if(New_State == Turn_Off) LED_0_Off();
		break;
		case LED_1_Device://LED1 ON KIT is PC7
		if(New_State == Turn_On) LED_1_On();
		else if(New_State == Turn_Off) LED_1_Off();
		break;
		case LED_2_Device://LED2 ON KIT is PD3
		if(New_State == Turn_On) LED_2_On();
		else if(New_State == Turn_Off) LED_2_Off();
		break;
		case LED_3_Device://LED3 RESERVE PIN PD4
		if(New_State == Turn_On) LED_3_On();
		else if(New_State == Turn_Off) LED_3_Off();
		break;
		case LED_4_Device://LED4 RESERVE PIN PD5
		if(New_State == Turn_On) LED_4_On();
		else if(New_State == Turn_Off) LED_4_Off();
		break;
	}
}

void LCD_GetUserID(uint8* userID){
    uint8 x = '\0';
    uint8 user_count = 0;
    LCD_Send_Clear_Screen();
    LCD_Send_String("Enter UserID:   ");
    while (user_count < 3) {
        x = '\0';x = KEYPAD_Get_Pressed_Key();if (x == '\0') continue;
        if (x >= '0' && x <= '9'){
        LCD_Send_Char(x);userID[user_count] = x;user_count++;}
        else if (x == '*' && user_count > 0){
        LCD_Delete_Last_Written();userID[user_count] = '\0';user_count--;}
        else if (x >= 'A' && x <= 'D') break;
    }
}
void LCD_GetPaswword(uint8* password){
    LCD_Send_Clear_Screen();
    LCD_Send_String("Enter Password: ");
    uint8 x = '\0';
    uint8 pass_count = 0;
    while (pass_count < 8) {
        x = '\0';x = KEYPAD_Get_Pressed_Key();if (x == '\0') continue;
        if (x >= '0' && x <= '9'){
        LCD_Send_Char(x);
		
		password[pass_count] = x;
		
		pass_count++;}
        else if (x == '*' && pass_count > 0){
        LCD_Delete_Last_Written();password[pass_count] = '\0';pass_count--;}
        else if (x >= 'A' && x <= 'D')break;
    }
}
uint8 Keypad_Get_User_Choice(){
	uint8 x = '\0';
	while (1) {
		x = '\0';x = KEYPAD_Get_Pressed_Key();if (x == '\0') continue;
		if (x >= '0' && x <= '9') return x;
	}
}

// LCD_Show_Welcome();
// LCD_Show_Welcome_User();
// LCD_Show_WrongUser();
// LCD_Show_WrongPassword();
// LCD_Show_Main_Options();
// LCD_Show_Choose_LED();
// LCD_Show_Choose_Dimmer();

void LCD_Handle_Choise(){
	// First we show user the options
	LCD_Show_Main_Options();
	// Then we wait for user choice
	uint8 x = Keypad_Get_User_Choice();
	LCD_Send_Clear_Screen();
	if(x == '1'){}//AC CODE
	else if (x == '2'){
        LCD_Send_String("UserID,username");
        LCD_Send_String("(0)More (9)Exit");
		//....
		}//User show list code
	else if (x == '3'){
		LCD_Show_Choose_LED();
		x = Keypad_Get_User_Choice();
		// ....
		}//Led Control Mode
	else if (x == '4'){
        LCD_Show_Choose_Dimmer();
		//Handle Dimmer choice ...
		}//Dimmer Light Control Mode
}
void LCD_Keypad_Login_Handler(){
	uint8 userID[3]; // MAX USER IS 3
	uint8 password[9];sint8 EEPROM_Pass[9];
	LCD_GetUserID(userID);
	if(userID[0] > '3' || userID[0] == '0' || userID[1] >= '0' || userID[2] >= '0') // Error user does not exist
	LCD_Show_WrongUser();
	//Now Search EEPROM For That User!!
	// if User is created it will have ID in EEPROM
	else if(EEPROM_Read_Byte(10 + ((userID[0]-48)*20) ) == (userID[0]-48)){
		// User Exist, Get Password From EEPROM
		EEPROM_Read_String(11 + ((userID[0]-48)*20) , EEPROM_Pass );// Getting Password from EEPROM
		// User Exist Process Next to GetPassword
		LCD_GetPaswword(password);
		//Now Get Password from User EEPROM !!
		// Now Flag the EEPROM THAT LCD USer Is Logged IN!
	}
}
void LCD_Idle(uint8 AC_State, uint8 Temp){
	//    LCD-KEYPAD-IDLE_NO_LOGIN
	//[AC(ON) Temp(30c)] -- [AC(OFF)Temp(20c)]
	//[****(0) To Login] -- [****(0) To Login]

	if(Temp >60) return;// Protection as max temp is 60 degree
	sint8 snum[3];
	itoa(Temp, snum, 10);
	LCD_Send_Clear_Screen(); // Clears anything on screen
	LCD_Send_String("AC(O"); // AC(O as it can be On or Off
	if     (AC_State == 0) LCD_Send_String("N) ");
	else if(AC_State == 1) LCD_Send_String("FF)");
	LCD_Send_String("Temp(");
	LCD_Send_String(snum);
	if     (Temp <  10) LCD_Send_String("c) ");
	else if(Temp >= 10) LCD_Send_String("c)");
	LCD_Send_String("        (0) More");
}

void Keypad_Check_If_Pressed_0(){}
	
void Factory_Reset(){
	//Restore default admin user and password
	//Restore All Default Settings
}




// UART_Show_MainMenu();
// UART_Show_Control_Appliances();
// UART_Show_Control_Leds();
// UART_Show_Control_AC();
// UART_Show_Control_Door();
// UART_Show_Control_Dimmer();
// UART_Show_UserManagement();
// UART_Show_Settings();
// UART_Show_User_List();
// UART_EEPROM_Delete_All_Users();


uint8 Global_UART_Flag_Username =0;
uint8 Global_UART_Flag_Password =0;




// step 1 user_id
// step 2 user_password
// step 3 choice_1(from main menu)
// step 4 choice_2(from Control_Appliances/UserManagement/Settings)
// step 5 choice_3(from ...)


uint8 global_Flag;
uint8 global_UART_Login_failed = 0;
sint8 global_UART_UserID[2];
sint8 global_UART_Username[8];
sint8 global_UART_Password[8];
sint8 global_EEPROM_Password[8];
sint8 global_UART_Choice_1[2];
sint8 global_UART_Choice_2[2];
sint8 global_UART_Choice_3[2];

interrupt(){
switch(global_Flag){
	case 1:
		GIE_Disable();
		UART_Show_Request_UserID();
		global_Flag++;
		GIE_Enable();
		break;
	case 2: //we got user input for userID now we manage it
		GIE_Disable();
		UART_Recieve_String_8(global_UART_UserID);
		if(EEPROM_Read_User(global_UART_UserID,global_UART_Username) == 0){
			//User Does not Exist
		}
		UART_Show_Request_Password();
		global_Flag++;
		GIE_Enable();
		break;
	case 3:
		GIE_Disable();
		UART_Recieve_String_8(global_UART_Password);
		if(EEPROM_Read_Password(global_UART_UserID,global_EEPROM_Password) == 0){
			//Password Does not Exist
		}
		else{
			//User Exists and password compare here
			UART_User_Login(uint8 * global_Flag, sint8 * global_UART_Username, sint8 * global_EEPROM_Password, sint8 * global_UART_Password);
		}
		GIE_Enable();
		break;
	case 4:
		GIE_Disable();
		UART_Recieve_String_8(global_UART_Choice_1);
		uint8 choice_1 = (global_UART_Choice_1[0]-48);
		if(choice_1 > 3 && choice_1 < 1)
		{
			UART_Show_Invalid();
			UART_Show_MainMenu();
			break;
		}
		else
		{
			UART_Show_MainMenu_Inside(choice_1);
			global_Flag++;
		}
		GIE_Enable();
		break;
	case 5:
		GIE_Disable();
		UART_Recieve_String_8(global_UART_Choice_2);
		uint8 choice_2 = (global_UART_Choice_2[0]-48);
		if(choice_2 > 6 && choice_2 < 1)
		{
			UART_Show_Invalid();
			UART_Show_MainMenu_Inside(choice_1);
		}
		else
		{
			switch(choice_1){
				case 1:			
					UART_Show_Control_Appliances_Inside(uint8 choice_2,sint8 * global_UART_UserID);
					break;
				case 2:					
				switch(choice_2){
					case 1:
						UART_Show_User_List();
						
						break;
					case 2:
						// Create NEW USER global_Flag++;
					break;
					case 3:
						//Delete Existing User global_Flag++;
					break;
					case 4:UART_EEPROM_Delete_All_Users();break;
					case 5:
						//Change User Password global_Flag++;
					break;
					case 6:	
						//Change User Username global_Flag++;
					break;
				}
				break;
				case 3:	
				switch(choice_2){
					case 1:
						//Test more options
					break;
					case 2:
						// Factory Reset
					break;
				}
				break;
			}
		}
		GIE_Enable();
		break;
		
	case 6:
		GIE_Disable();
	
		GIE_Enable();
		break;
}

}








uint8 UART_Get_User_Pass(){
	sint8 EEPROM_username[8];
	sint8 EEPROM_password[8];
	uint8 Entered_UserID;
	uint8 Entered_password[8];
	UART_Send_String_Polling_8("Please Enter UserID:");
	// RECEIVE USER ID
	
	// SEARCH USER ID
	if(EEPROM_Read_User(Entered_UserID,EEPROM_username) == 0){
		//User Does not Exist
	}else{
		//User Exists
	}
	UART_Send_String_Polling_8("Please Enter Password:");
	// RECEIVE PASSWORD
	
	// COMPARE PASSWORD
	if(EEPROM_Read_Password(Entered_UserID,EEPROM_password) == 0){
		//User Does not Exist
		}else{
		//User Exists
	}
	uint8 UserID_state;
	// 250 means failed to login
	// 0 means admin login success
	// 1 means first user login success ...
	return UserID_state;
}




void UART_EEPROM_Reg_New_User(){
	//EEPROM_Reg_New_User();
}


/*
void UART_Handler(){
	uint8 UserID = 200;
	// Ask for login 3 times if not lock the system
	for(int i =0; i < 3;i++)
	{
		if(i != 0 && UserID == 250) 
		{
			UART_Send_String_Polling_8("Login Failed Please Try Again \n");
		}
		UserID = UART_Get_User_Pass();
		if(UserID != 250 && UserID != 200){
			// Means login success and returned the UserID
			break;
		}
	}
	
	if(UserID == 200){}// ERROR Goes Here
	else if(UserID == 250)
	{
		// Login Failed 3 times Break the system Here
		BUZZER_ALARM_TILL_RESET();
	}
	else{
		uint8 Choice_1 = 0, Choice_2 = 0, Choice_3 = 0;

		UART_Send_String_Polling_8("Login Success \n");
		UART_Show_MainMenu();
		//Something here will change choice
		while(Choice_1 > 3 && Choice_1 < 1)
			UART_Show_MainMenu();
			//Something here will change choice
		switch(Choice_1){
			case 1: 
				UART_Show_Control_Appliances();
				//Something here will change choice
				switch(Choice_2){
					case 1: UART_Show_Control_Leds(); break;
					case 2: UART_Show_Control_AC(); break;
					case 3: UART_Show_Control_Door(); break;
					case 4: UART_Show_Control_Dimmer(); break;
				}
				break;
			case 2: 
				UART_Show_UserManagement();
				//Something here will change choice
				switch(Choice_2){
					case 1: UART_Show_User_List(); break; // Only show no interaction Needed
					case 2: UART_EEPROM_Reg_New_User(); break;
					case 3: UART_Show_Control_Door(); break;
					case 4: UART_EEPROM_Delete_All_Users(); break; // Only show no interaction Needed
					case 5: UART_Show_Control_Dimmer(); break;
					case 6: UART_Show_Control_Dimmer(); break;
				}
				break;
			case 3: 
					UART_Show_Settings();
					//Something here will change choice
					switch(Choice_3){
						case 1:  break; 
						case 2:  break;
					}
					break;
		}
	}
}

*/

void Smart_Idle(){
    // There are 3 things should always work at idle
    // 1) The idle Screen on LCD ( Idle > Login > options > ... > Idle )
    // 2) The door Servo always works (servo implementation with timer+Interrupt)
    // 3) waiting for user input on UART Bluetooth (Cant use Polling method!! Interrupt)
    // 4) Waiting for User input on Keypad:  KEYPAD_Get_Pressed_Key();
	
	//YOUSSEF >> DOOR SERVO WITH TIMER1 + INTERRUPT
	//YOUSSEF >> POT + INTERRUPT

	//HOW TO MAKE THEM ALL WORK TOGETHER!!!

}

