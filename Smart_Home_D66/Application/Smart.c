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
#include <avr/interrupt.h>

/****************      Buzzer Include               ********************/
#include "../HAL/Buzzer/Buzzer.h"                      //Done
/****************      KEYPAD Include               ********************/
#include "../HAL/Keypad_4x4/Keypad_4x4.h"              //Done
/****************      AirConditioner Include       ********************/
#include "../HAL/AirConditioner/AC.h"                  //Done
/****************      5 Leds Include               ********************/
#include "../HAL/Leds_5/Leds.h"                        //Done
/****************      EEPROM Include               ********************/
#include "../HAL/EEPROM/EEPROM.h"                      //Done
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


// LCD_Show_Welcome();
// LCD_Show_Welcome_User();
// LCD_Show_WrongUser();
// LCD_Show_WrongPassword();
// LCD_Show_Main_Options();
// LCD_Show_Choose_LED();
// LCD_Show_Choose_Dimmer();


void Keypad_Check_If_Pressed_0(){}



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

