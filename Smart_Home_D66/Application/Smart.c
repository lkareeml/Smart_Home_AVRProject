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
void LCD_Show_Welcome(){
    //  [Welcome To *****]
    //  [Kiak Smart home*]
    LCD_Send_Clear_Screen();
    LCD_Send_String("Welcome To      ");
    LCD_Send_String("Smart home App  ");
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
void LCD_Show_Welcome_User(){
	LCD_Send_Clear_Screen();
    LCD_Send_String("Welcome Back!   ");
    LCD_Send_String("Username        ");
}
void LCD_Show_WrongUser(){
	//In case you Can't find username :
	LCD_Send_Clear_Screen();
	LCD_Send_String("Wrong User      ");
	LCD_Send_String("                ");
}
void LCD_Show_WrongPassword(){
	//In case Wrong Password :
	LCD_Send_Clear_Screen();
	LCD_Send_String("Wrong Password  ");
	LCD_Send_String("                ");
}
void LCD_Show_Main_Options(){
	LCD_Send_Clear_Screen();
    LCD_Send_String("(1)AC  (2)User  ");
    LCD_Send_String("(3)Led (4)Dimmer");
}
void LCD_Show_Choose_LED(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Led No.(1)(2)(3)");
	LCD_Send_String("(4)(5)   (0)Exit");
}
void LCD_Show_Choose_Dimmer(){
	LCD_Send_Clear_Screen();
	LCD_Send_String("Dimmer Higher(1)");
	LCD_Send_String("(0)Low   (9)Exit");
}
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

uint8 EEPROM_Reg_User(uint8 UserID, sint8 * Username, sint8 * Password){
	uint16 Marker = (UserID*20) + 10;
	if(EEPROM_Read_Byte(Marker) != 0){
		return 0; // Means User already exist
	}
	else{// User does not exist, Create the user
		EEPROM_Write_Byte(Marker,UserID);
		EEPROM_Write_String((Marker+1),Password);
		EEPROM_Write_Byte(Marker+9, 0 );
		EEPROM_Write_String((Marker+10),Username);
		EEPROM_Write_Byte(Marker+18, 0 );
		EEPROM_Write_Byte(Marker+19, 0 );
		return 1;// Means User Successfully Registered
	}
}
uint8 EEPROM_Edit_User_Password(uint8 UserID,sint8 * Password){
	uint16 Marker = (UserID*20) + 10;
	if(EEPROM_Read_Byte(Marker) == 0){
		return 0; // Means User Does not exist
	}
	else{// User does exist, Change Password
		for(uint16 Address =(Marker+1);Address<=(Marker+9);Address++)
		{
			EEPROM_Write_Byte(Address,0);
		}
		EEPROM_Write_String((Marker+1),Password);
		return 1;// Means User Successfully Change Password
	}
}
uint8 EEPROM_Edit_Username(uint8 UserID,sint8 * Username){
	uint16 Marker = (UserID*20) + 10;
	if(EEPROM_Read_Byte(Marker) == 0){
		return 0; // Means User Does not exist
	}
	else{// User does exist, Change Password
		for(uint16 Address =(Marker+10);Address<=(Marker+18);Address++)
		{
			EEPROM_Write_Byte(Address,0);
		}
		EEPROM_Write_String((Marker+10),Username);
		return 1;// Means User Successfully Change Password
	}
}
uint8 EEPROM_Read_User(uint8 UserID,uint8 * Username){
	uint16 Marker = (UserID*20) + 10;
	if(EEPROM_Read_Byte(Marker) == 0){
		return 0; // Means User Dose Not exist
	}else{
		EEPROM_Read_String(Marker+10,Username);
		return 1;
	}
}
uint8 EEPROM_Delete_User(uint8 UserID){
	uint16 Marker = (UserID*20) + 10;
	if(EEPROM_Read_Byte(Marker) == 0){
		return 0; // Means User Dose Not exist
		}else{
		for(uint16 Address = Marker; Address < (Marker + 20);Address++){
			EEPROM_Write_Byte(Address,0);
			return 1;// Means User Deleted Successfully
		}
	}
}


void UART_EEPROM_Show_User_List(){
	for(uint8 id = 0;id<4;id++){
		uint8 Username[8];
		if(EEPROM_Read_User(id,Username)){
			UART_Send_String_Polling_8("UserID: ");
			UART_Send_Byte_Polling_8(id);
			UART_Send_String_Polling_8(" Username: ");
			UART_Send_String_Polling_8(Username);
			UART_Send_String_Polling_8("/n");
		}
	}
}
void UART_EEPROM_Delete_All_Users(){
	for(uint8 id = 0;id<4;id++){
		if(EEPROM_Delete_User(id)){
			UART_Send_String_Polling_8("UserID: ");
			UART_Send_Byte_Polling_8(id);
			UART_Send_String_Polling_8(" Successfully\n");
		}
	}
	UART_Send_String_Polling_8("Deleted All Users Successfully\n");
}












void UART_Show_Options(){
	
}



void UART_Handler(){
	
	// UART_Send_String_Polling_8("STRING HERE")
	
	// >>> Welcome, Please Enter UserID:
	// RECEIVE USER ID
	// SEARCH USER ID
	// >>> Please Enter Password:
	// RECEIVE PASSWORD
	// COMPARE PASSWORD
	
	// LABLE X
	// Success Login: >> "Login Success"
		// >>> "Choose Option: "
		// >>> "001) Control Appliances"
			// >>> "011) Control Leds"
				// >>> "Led 1 (11) On,(01) Off"
				// >>> "Led 2 (12) On,(02) Off"
				// >>> "Led 3 (13) On,(03) Off"
				// >>> "Led 4 (14) On,(04) Off"
				// >>> "Led 5 (15) On,(05) Off"
			// >>> "021) Control AC"
				// >>> "AC Auto (11)"
				// >>> "AC Manual Turn Off (02)"
				// >>> "AC Manual Turn On  (12)"
			// >>> "031) Control Door"
				// >>> "1) Open Door Lock"
				// >>> "2) Close Door Lock"
			// >>> "041) Control Dimmer"
				// >>> "1) Dimmer Up"
				// >>> "2) Dimmer Down"
				// >>> "3) Dimmer Off"
				// >>> "4) Dimmer On"
					
		// >>> "002) User Management"
			// >>> "012) Show Users list"
			// >>> "022) Create New User"
			// >>> "032) Delete Existing User"
			// >>> "042) Delete All Users"
			// >>> "052) Change User Password"
			// >>> "062) Change User Username"

		// >>> "003) Settings"
			// >>> "013) Date and Time (dev)"
			// >>> "023) Test Buzzer"
			// >>> "033) Factory Reset"
			
	// Failed Login: >> "Login Failed Try Again"
		// >>> Please Enter UserID:
		// RECEIVE USER ID
		// SEARCH USER ID
		// >>> Please Enter Password:
		// RECEIVE PASSWORD
		// COMPARE PASSWORD
}


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

