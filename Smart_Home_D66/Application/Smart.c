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
#include "../MCAL/UART/UART.h"                              //Done
#include "../MCAL/Analog_To_Digital_Converter/ADC.h"        //Done
#include "../MCAL/TWI_I2C/TWI_I2C.h"                        //Done
#include "Smart.h"                                          //Done
#include <stdlib.h>
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
void LCD_Start_App(){
    LCD_Send_Clear_Screen();
    //    [Welcome To *****]
    //  [Kiak Smart home*]
    LCD_Send_String("Welcome To      ");
    LCD_Send_String("Smart home App  ");
}
void LCD_GetUsername(uint8* username){
    uint8 x = '\0';
    uint8 user_count = 0;
    LCD_Send_Clear_Screen();
    LCD_Send_String("Enter UserID:   ");
    while (user_count < 8) {
        x = '\0';x = KEYPAD_Get_Pressed_Key();if (x == '\0') continue;
        if (x >= '0' && x <= '9'){
        LCD_Send_Char(x);username[user_count] = x;user_count++;}
        else if (x == '*' && user_count > 0){
        LCD_Delete_Last_Written();username[user_count] = '\0';user_count--;}
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
uint8 LCD_Get_User_Choice(){
	uint8 x = '\0';
	while (1) {
		x = '\0';x = KEYPAD_Get_Pressed_Key();if (x == '\0') continue;
		if (x >= '0' && x <= '9') return x;
	}
}
void LCD_Welcome(){
	LCD_Send_Clear_Screen();
    LCD_Send_String("Welcome Back!   ");
    LCD_Send_String("Username        ");
}
void LCD_Idle(uint8 AC_State, uint8 Temp){
    //    LCD-KEYPAD-IDLE_NO_LOGIN
    //[AC(ON) Temp(30c)] -- [AC(OFF)Temp(20c)]
    //[****(0) To Login] -- [****(0) To Login]
    
    if(Temp >100) return;// Protection as max temp is 60 degree
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
void LCD_Keypad_Login_Handler(){
	uint8 username[9];
	uint8 password[9];

	LCD_GetUsername(username);
	//Now Search EEPROM For That User!!
	
	//In case you Can't find username : LCD_Send_String("Wrong User      ");
	//In case you found username :      LCD_Send_String("Correct User    ");

	LCD_GetUsername(password);
	//Now Get Password from User EEPROM !!
	
	//In case Password Doesn't match : LCD_Send_String ("Wrong Password  ");
	//In case Password match         : LCD_Send_String ("Correct Password");

	// Now Flag the EEPROM THAT LCD USer Is Logged IN!
	
	//
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
	uint8 x = LCD_Get_User_Choice();
	LCD_Send_Clear_Screen();
	if(x == '1'){}//AC CODE
	else if (x == '2'){
        LCD_Send_String("UserID,username");
        LCD_Send_String("(0)More (9)Exit");
		//....
		}//User show list code
	else if (x == '3'){
		LCD_Show_Choose_LED();
		x = LCD_Get_User_Choice();
		// ....
		}//Led Control Mode
	else if (x == '4'){
        LCD_Show_Choose_Dimmer();
		//Handle Dimmer choice ...
		}//Dimmer Light Control Mode
}

void Smart_Idle(){
    // There are 3 things should always work at idle
    // 1) The idle Screen on LCD ( Idle > Login > options > ... > Idle )
    // 2) The door Servo always works (servo implementation with timer+Interrupt)
    // 3) waiting for user input on UART Bluetooth (Cant use Polling method!! Interrupt)
    // 4) Waiting for User input on Keypad:  KEYPAD_Get_Pressed_Key();
	
	//HOW TO MAKE THEM ALL WORK TOGETHER!!!

}

