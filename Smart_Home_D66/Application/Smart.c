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
Timer2 >> 

******************************************************************************
******************************************************************************
Some Notes: 

Login system admin and user “admin is remoted only.
--->    Admin mode can register any user or remove.
--->    Usernames and passwords kept in EEPROM
--->    user/passwords wrong 3 trial,alarm until reset.
--->    door Control for Admin Only
--->    Create / Delete  for Admin Only{EEPROM}
--->    USER control even any user login remote, except admin until allowing of admin
--->    LCD must display (IDLE)
--->    user’s usernames LCD different user’s usernames in the UART system
--->    Temperature Sensor if temp > 28 °C, Air condition TURN_ON DC_Motor
--->    Temperature Sensor if temp < 21 °C, Air condition    TURN_OFF DC_Motor

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

